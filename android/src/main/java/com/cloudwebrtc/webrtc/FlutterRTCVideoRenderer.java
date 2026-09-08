package com.cloudwebrtc.webrtc;

import android.util.Log;
import android.graphics.SurfaceTexture;
import android.view.Surface;

import com.cloudwebrtc.webrtc.utils.AnyThreadSink;
import com.cloudwebrtc.webrtc.utils.ConstraintsMap;
import com.cloudwebrtc.webrtc.utils.EglUtils;

import java.util.List;

import org.webrtc.EglBase;
import org.webrtc.GlRectDrawer;
import org.webrtc.MediaStream;
import org.webrtc.RendererCommon;
import org.webrtc.RendererCommon.RendererEvents;
import org.webrtc.VideoTrack;

import io.flutter.plugin.common.EventChannel;
import io.flutter.view.TextureRegistry;

public class FlutterRTCVideoRenderer implements EventChannel.StreamHandler {

    private static final String TAG = FlutterWebRTCPlugin.TAG;
    private final TextureRegistry.SurfaceProducer producer;
    private int id = -1;
    private String mediaStreamId;

    private String ownerTag;

    public void Dispose() {
        setVideoTrack(null);
        if (surfaceTextureRenderer != null) {
            surfaceTextureRenderer.release();
        }
        if (eventChannel != null)
            eventChannel.setStreamHandler(null);

        eventSink = null;
        producer.release();
    }

    /**
     * The {@code RendererEvents} which listens to rendering events reported by
     * {@link #surfaceTextureRenderer}.
     */
    private RendererEvents rendererEvents;

    /**
     * Last raw decoder frame size seen (pre-dewarp), cached so {@link
     * #setDewarpConfig} can immediately recompute the composite size without
     * waiting for a genuine raw-resolution change. Written from the render
     * thread, read from the main thread, hence volatile.
     */
    private volatile int lastRawWidth = 0;
    private volatile int lastRawHeight = 0;

    /**
     * Last composite size / rotation actually reported to the Dart side via
     * {@code didTextureChangeVideoSize} / {@code didTextureChangeRotation}.
     * Only touched inside the synchronized {@code maybeReport*} methods below,
     * which both {@link #onFrameResolutionChanged} (render thread) and {@link
     * #setDewarpConfig} (main thread) call into.
     */
    private int reportedWidth = 0;
    private int reportedHeight = 0;
    private int reportedRotation = -1;

    private void listenRendererEvents() {
        rendererEvents = new RendererEvents() {
            @Override
            public void onFirstFrameRendered() {
                ConstraintsMap params = new ConstraintsMap();
                params.putString("event", "didFirstFrameRendered");
                params.putInt("id", id);
                if (eventSink != null) {
                    eventSink.success(params.toMap());
                }
            }

            @Override
            public void onFrameResolutionChanged(
                    int videoWidth, int videoHeight,
                    int rotation) {
                lastRawWidth = videoWidth;
                lastRawHeight = videoHeight;
                maybeReportSizeChange(videoWidth, videoHeight);
                maybeReportRotationChange(rotation);
            }
        };
    }

    /**
     * Recomputes the reported size for raw decoder frame {@code (rawWidth,
     * rawHeight)} -- the composite canvas size when {@link #dewarpConfig} is
     * set, the raw size otherwise -- and notifies the Dart side if it differs
     * from what was last reported, so {@code RTCVideoView}'s aspect-ratio-driven
     * layout stays correct.
     *
     * Called from two places: {@link #onFrameResolutionChanged} on the render
     * thread whenever the *raw* decoder resolution/rotation genuinely changes,
     * and {@link #setDewarpConfig} on the main thread. The second call is what
     * makes toggling dewarp mid-stream work: the composite size can change
     * without the raw decoder resolution changing at all, so relying solely on
     * {@link #onFrameResolutionChanged} would leave the Dart side holding a
     * stale aspect ratio -- stretching/squashing the video -- until some
     * unrelated raw resolution change happened to come along, if ever.
     */
    private synchronized void maybeReportSizeChange(int rawWidth, int rawHeight) {
        if (eventSink == null || rawWidth <= 0 || rawHeight <= 0) return;

        DewarpConfig currentDewarpConfig = dewarpConfig;
        int width = rawWidth;
        int height = rawHeight;
        if (currentDewarpConfig != null) {
            int[] compositeSize = DewarpGlDrawer.compositeSize(
                    currentDewarpConfig, rawWidth, rawHeight);
            width = compositeSize[0];
            height = compositeSize[1];
        }
        if (reportedWidth == width && reportedHeight == height) return;

        reportedWidth = width;
        reportedHeight = height;
        ConstraintsMap params = new ConstraintsMap();
        params.putString("event", "didTextureChangeVideoSize");
        params.putInt("id", id);
        params.putDouble("width", (double) width);
        params.putDouble("height", (double) height);
        eventSink.success(params.toMap());
    }

    private synchronized void maybeReportRotationChange(int rotation) {
        if (eventSink == null || reportedRotation == rotation) return;
        reportedRotation = rotation;
        ConstraintsMap params = new ConstraintsMap();
        params.putString("event", "didTextureChangeRotation");
        params.putInt("id", id);
        params.putInt("rotation", rotation);
        eventSink.success(params.toMap());
    }

    private final SurfaceTextureRenderer surfaceTextureRenderer;

    /**
     * The {@code VideoTrack}, if any, rendered by this {@code FlutterRTCVideoRenderer}.
     */
    private VideoTrack videoTrack;
    private String videoTrackId;

    /**
     * Fisheye dewarp configuration set via
     * {@link #setDewarpConfig(DewarpConfig)}, or {@code null} for the
     * default passthrough rendering. Read from the render thread (by the
     * {@code onFrameResolutionChanged} callback above) and written from the
     * main thread, hence volatile.
     */
    private volatile DewarpConfig dewarpConfig;

    EventChannel eventChannel;
    EventChannel.EventSink eventSink;

    public FlutterRTCVideoRenderer(TextureRegistry.SurfaceProducer producer) {
        this.surfaceTextureRenderer = new SurfaceTextureRenderer("");
        listenRendererEvents();
        surfaceTextureRenderer.init(
                EglUtils.getRootEglBaseContext(), rendererEvents, EglBase.CONFIG_PLAIN, createDrawer());
        surfaceTextureRenderer.surfaceCreated(producer);

        this.eventSink = null;
        this.producer = producer;
        this.ownerTag = null;
    }

    /**
     * Configures (or, passing {@code null}, clears) real-time fisheye lens
     * dewarping for this renderer.
     *
     * This never disconnects or renegotiates anything at the WebRTC layer
     * — {@link #videoTrack} itself, the underlying {@code MediaStreamTrack}
     * and the peer connection it belongs to are all left completely alone.
     * {@code removeRendererFromVideoTrack()}/{@code tryAddRendererToVideoTrack()}
     * below only detach and reattach this renderer's own local
     * {@code VideoSink} from the (unchanged) track — that's purely a local
     * rendering-pipeline operation (needed because there is no way to swap
     * a {@code GlDrawer} on a live {@code EglRenderer}; swapping requires a
     * release()/init() cycle). The remote peer keeps sending frames the
     * entire time; at most a frame or two is dropped locally during the
     * brief detach window, not a stream interruption. Safe to call
     * whenever a stream is already playing.
     */
    public void setDewarpConfig(DewarpConfig config) {
        this.dewarpConfig = config;
        // The composite size can change even though the raw decoder
        // resolution hasn't -- proactively re-report it now instead of
        // waiting for the next onFrameResolutionChanged, which is driven by
        // the raw decoder and may not fire again for a long time, or ever.
        if (lastRawWidth > 0 && lastRawHeight > 0) {
            maybeReportSizeChange(lastRawWidth, lastRawHeight);
        }
        if (videoTrack != null) {
            removeRendererFromVideoTrack();
            try {
                tryAddRendererToVideoTrack();
            } catch (Exception e) {
                Log.e(TAG, "setDewarpConfig " + e);
            }
        }
    }

    private RendererCommon.GlDrawer createDrawer() {
        DewarpConfig config = this.dewarpConfig;
        return config == null ? new GlRectDrawer() : new DewarpGlDrawer(config);
    }

    public void setEventChannel(EventChannel eventChannel) {
        this.eventChannel = eventChannel;
    }

    public void setId(int id) {
        this.id = id;
    }

    @Override
    public void onListen(Object o, EventChannel.EventSink sink) {
        eventSink = new AnyThreadSink(sink);
    }

    @Override
    public void onCancel(Object o) {
        eventSink = null;
    }

    /**
     * Stops rendering {@link #videoTrack} and releases the associated acquired
     * resources (if rendering is in progress).
     */
    private void removeRendererFromVideoTrack() {
        if (videoTrack == null) {
            return;
        }
        try {
            videoTrack.removeSink(surfaceTextureRenderer);
        } catch (IllegalStateException e) {
            Log.w(TAG, "VideoTrack was disposed before its renderer was removed");
        }
    }

    /**
     * Sets the {@code MediaStream} to be rendered by this {@code FlutterRTCVideoRenderer}.
     * The implementation renders the first {@link VideoTrack}, if any, of the
     * specified {@code mediaStream}.
     *
     * @param mediaStream The {@code MediaStream} to be rendered by this
     *                    {@code FlutterRTCVideoRenderer} or {@code null}.
     */
    public void setStream(MediaStream mediaStream, String ownerTag) {
        VideoTrack videoTrack;
        this.mediaStreamId = mediaStream == null ? null : mediaStream.getId();
        this.ownerTag = ownerTag;
        if (mediaStream == null) {
            videoTrack = null;
        } else {
            List<VideoTrack> videoTracks = mediaStream.videoTracks;

            videoTrack = videoTracks.isEmpty() ? null : videoTracks.get(0);
        }

        setVideoTrack(videoTrack);
    }
   /**
     * Sets the {@code MediaStream} to be rendered by this {@code FlutterRTCVideoRenderer}.
     * The implementation renders the first {@link VideoTrack}, if any, of the
     * specified trackId
     *
     * @param mediaStream The {@code MediaStream} to be rendered by this
     *                    {@code FlutterRTCVideoRenderer} or {@code null}.
     * @param trackId The {@code trackId} to be rendered by this
     *                    {@code FlutterRTCVideoRenderer} or {@code null}.
     */
    public void setStream(MediaStream mediaStream,String trackId, String ownerTag) {
        VideoTrack videoTrack;
        this.mediaStreamId = mediaStream == null ? null : mediaStream.getId();
        this.ownerTag = ownerTag;
        if (mediaStream == null) {
            videoTrack = null;
        } else {
            List<VideoTrack> videoTracks = mediaStream.videoTracks;

            videoTrack = videoTracks.isEmpty() ? null : videoTracks.get(0);

            for (VideoTrack track : videoTracks){
                if (track.id().equals(trackId)){
                    videoTrack = track;
                }
            }
        }

        setVideoTrack(videoTrack);
    }

    /**
     * Sets a video track already resolved from its owning PeerConnection.
     *
     * The stream id is retained only for renderer lifecycle bookkeeping. The
     * MediaStream wrapper itself may be replaced or disposed by a Unified Plan
     * renegotiation while the logical stream and track ids remain unchanged.
     */
    public void setTrack(
            VideoTrack videoTrack, String mediaStreamId, String ownerTag) {
        this.mediaStreamId = mediaStreamId;
        this.ownerTag = ownerTag;
        setVideoTrack(videoTrack);
    }

    /**
     * Sets the {@code VideoTrack} to be rendered by this {@code FlutterRTCVideoRenderer}.
     *
     * @param videoTrack The {@code VideoTrack} to be rendered by this
     *                   {@code FlutterRTCVideoRenderer} or {@code null}.
     */
    public void setVideoTrack(VideoTrack videoTrack) {
        VideoTrack oldValue = this.videoTrack;

        if (oldValue != videoTrack) {
            if (oldValue != null) {
                removeRendererFromVideoTrack();
            }

            this.videoTrack = videoTrack;
            this.videoTrackId = videoTrack == null ? null : videoTrack.id();

            if (videoTrack != null) {
                try {
                    Log.w(TAG, "FlutterRTCVideoRenderer.setVideoTrack, set video track to " + videoTrack.id());
                    tryAddRendererToVideoTrack();
                } catch (Exception e) {
                    Log.e(TAG, "tryAddRendererToVideoTrack " + e);
                }
            } else {
                Log.w(TAG, "FlutterRTCVideoRenderer.setVideoTrack, set video track to null");
            }
        }
    }

    /**
     * Starts rendering {@link #videoTrack} if rendering is not in progress and
     * all preconditions for the start of rendering are met.
     */
    private void tryAddRendererToVideoTrack() throws Exception {
        if (videoTrack != null) {
            EglBase.Context sharedContext = EglUtils.getRootEglBaseContext();

            if (sharedContext == null) {
                // If SurfaceViewRenderer#init() is invoked, it will throw a
                // RuntimeException which will very likely kill the application.
                Log.e(TAG, "Failed to render a VideoTrack!");
                return;
            }

            surfaceTextureRenderer.release();
            listenRendererEvents();
            surfaceTextureRenderer.init(
                    sharedContext, rendererEvents, EglBase.CONFIG_PLAIN, createDrawer());
            surfaceTextureRenderer.surfaceCreated(producer);

            videoTrack.addSink(surfaceTextureRenderer);
        }
    }

    public boolean checkMediaStream(String id, String ownerTag) {
        if (null == id || null == mediaStreamId || ownerTag == null || !ownerTag.equals(this.ownerTag)) {
            return false;
        }
        return id.equals(mediaStreamId);
    }

    public boolean checkVideoTrack(String id, String ownerTag) {
        if (null == id || null == videoTrackId || ownerTag == null || !ownerTag.equals(this.ownerTag)) {
            return false;
        }
        return id.equals(videoTrackId);
    }
}
