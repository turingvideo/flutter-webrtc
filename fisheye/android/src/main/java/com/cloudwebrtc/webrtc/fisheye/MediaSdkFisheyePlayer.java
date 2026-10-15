package com.cloudwebrtc.webrtc.fisheye;

import android.util.Log;

import com.cloudwebrtc.webrtc.FlutterWebRTCPlugin;
import com.cloudwebrtc.webrtc.SurfaceTextureRenderer;
import com.cloudwebrtc.webrtc.utils.ConstraintsMap;
import com.cloudwebrtc.webrtc.utils.EglUtils;

import org.webrtc.JavaI420Buffer;
import org.webrtc.VideoFrame;

import java.nio.ByteBuffer;

import io.flutter.view.TextureRegistry;

/**
 * Thin JNI wrapper around the vendor MediaSDK (fisheye/android/vendor,
 * headers in fisheye/android/vendor/include) -- mirrors the reference C
 * call sequence from fisheye/README.md: Init -> CreatPortRes ->
 * SetDecodeVideoDataCB -> SetVideoCallBackMode(PROCESSED_DATA) ->
 * StartStream(RTSP realplay, pWnd=NULL) -> SetPtzAndFixMode -> ... ->
 * StopStream -> DestroyPortRes -> Cleanup. The actual calls live in the
 * JNI glue at fisheye/android/src/main/cpp/mediasdk_jni.cpp.
 *
 * Frame arrival is also reported via FlutterWebRTC's existing shared
 * event channel (see onFrame() below) so it's observable from Dart
 * independent of whether a texture is attached.
 *
 * Rendering reuses WebRTC's own EGL pipeline (SurfaceTextureRenderer,
 * the same class FlutterRTCVideoRenderer already drives) rather than a
 * new GL setup: each corrected YUV420 frame is copied into a
 * JavaI420Buffer, wrapped in a VideoFrame, and fed to
 * SurfaceTextureRenderer.onFrame() -- no fisheye-specific GL drawer is
 * needed here since MediaSDK already did the dewarping; this only has
 * to display an already-correct rectangular frame.
 */
public class MediaSdkFisheyePlayer {
  private static final String TAG = "MediaSdkFisheyePlayer";

  // MEDIASDK_PTZ_MODE_E (media_sdk_com_def.h) -- only the modes this
  // project currently cares about; see the header for the full list.
  public static final int PTZ_MODE_ORIGINAL = 0;
  public static final int PTZ_MODE_180 = 1;
  public static final int PTZ_MODE_360_1PTZ = 2;
  public static final int PTZ_MODE_360_6PTZ = 3;

  // MEDIASDK_FIX_MODE_E
  public static final int FIX_MODE_TOP_INSTALLATION = 0;
  public static final int FIX_MODE_BOTTOM_INSTALLATION = 1;
  public static final int FIX_MODE_SIDE_INSTALLATION = 2;

  private static boolean sdkInited = false;
  private static SurfaceTextureRenderer surfaceTextureRenderer;

  private int port = -1;
  private String lastRtspUrl;

  static {
    System.loadLibrary("fisheye_mediasdk_jni");
  }

  // Each native method returns MediaSDK's own ULONG_32 error code
  // (0 == ERR_MEDIASDK_SUCCEED), except nativeCreatePort which writes the
  // new port number into outPort[0] and returns the SDK error code.
  private static native int nativeInit();
  private static native int nativeCleanup();
  private static native int nativeCreatePort(int[] outPort);
  private static native int nativeDestroyPort(int port);
  private static native int nativeStartRtspStream(int port, String rtspUrl);
  private static native int nativeStopStream(int port);
  private static native int nativeSetPtzAndFixMode(int port, int ptzMode, int fixMode);
  private static native int nativeSetDecodeVideoDataCB(int port);

  private static synchronized boolean ensureInited() {
    if (sdkInited) return true;
    int rc = nativeInit();
    sdkInited = (rc == 0);
    if (!sdkInited) {
      Log.e(TAG, "MediaSDK_Init failed, rc=" + rc);
    }
    return sdkInited;
  }

  /**
   * Starts pulling the given RTSP URL and applying the given fisheye
   * correction mode. {@code ptzMode}/{@code fixMode} are the PTZ_MODE_
   * and FIX_MODE_ constants above (mirroring MEDIASDK_PTZ_MODE_E and
   * MEDIASDK_FIX_MODE_E exactly). Returns true if every setup call
   * succeeded; check logcat (tag "MediaSdkFisheyePlayer") for the exact
   * failing step otherwise.
   */
  public synchronized boolean start(String rtspUrl, int ptzMode, int fixMode) {
    if (!ensureInited()) return false;
    if (port != -1) {
      Log.w(TAG, "start() called while already running on port " + port + " -- stopping it first");
      stop();
    }

    int[] outPort = new int[1];
    int rc = nativeCreatePort(outPort);
    if (rc != 0) {
      Log.e(TAG, "MediaSDK_CreatPortRes failed, rc=" + rc);
      return false;
    }
    port = outPort[0];

    rc = nativeSetDecodeVideoDataCB(port);
    if (rc != 0) {
      Log.e(TAG, "MediaSDK_SetDecodeVideoDataCB/SetVideoCallBackMode failed, rc=" + rc);
      nativeDestroyPort(port);
      port = -1;
      return false;
    }

    rc = nativeStartRtspStream(port, rtspUrl);
    if (rc != 0) {
      Log.e(TAG, "MediaSDK_StartStream failed, rc=" + rc);
      nativeDestroyPort(port);
      port = -1;
      return false;
    }

    // CONFIRMED via real-device testing (2026-10-14): calling this BEFORE
    // StartStream -- tried on the theory that the correction pipeline
    // might need setting up before the first frame -- made MediaSDK
    // reject it outright (err 0x11d28, rc=73000), unlike the silent
    // no-op/no-visual-effect behavior seen calling it after start. So the
    // port needs an active realplay session before its PTZ/fix mode can
    // be set at all; fisheye/README.md's reference order (after
    // StartStream) is the only one that even accepts the call, even
    // though -- per the *separate*, already-confirmed finding -- the
    // output frames don't actually show a corrected image either way.
    rc = nativeSetPtzAndFixMode(port, ptzMode, fixMode);
    if (rc != 0) {
      // Not fatal to the stream itself -- leave it running so the raw
      // (uncorrected) circle is still watchable/debuggable.
      Log.e(TAG, "MediaSDK_SetPtzAndFixMode failed, rc=" + rc);
    }
    lastRtspUrl = rtspUrl;
    return true;
  }

  /**
   * EXPERIMENT (2026-10-14): vendor support's suggestion was "after
   * changing the fisheye correction mode, you need to restart the video
   * stream" ("修改鱼眼矫正模式后，需要重新启动视频流"). We'd already tried
   * (a) calling SetPtzAndFixMode on a live stream with no restart at all,
   * and (b) a full teardown+recreate of the port with the new mode baked
   * into a brand-new StartStream call -- neither showed any correction
   * effect. This tries the one combination in between those two: set the
   * new mode on the existing port, then StopStream + StartStream *on that
   * same port* (no DestroyPortRes/CreatPortRes), in case the correction
   * pipeline only reconfigures itself during StartStream's own setup and
   * reads whatever PTZ/fix mode was last set on the port, rather than
   * reacting to SetPtzAndFixMode directly.
   */
  public synchronized boolean restartSamePortForModeChange(int ptzMode, int fixMode) {
    if (port == -1 || lastRtspUrl == null) {
      Log.w(TAG, "restartSamePortForModeChange() called with no active stream");
      return false;
    }
    int rc = nativeSetPtzAndFixMode(port, ptzMode, fixMode);
    Log.i(TAG, "restartSamePortForModeChange: SetPtzAndFixMode rc=" + rc);

    rc = nativeStopStream(port);
    Log.i(TAG, "restartSamePortForModeChange: StopStream rc=" + rc);

    rc = nativeStartRtspStream(port, lastRtspUrl);
    Log.i(TAG, "restartSamePortForModeChange: StartStream rc=" + rc);
    return rc == 0;
  }

  /**
   * Changes the fisheye correction mode on the already-running stream,
   * without tearing down and re-opening the RTSP connection -- unlike
   * {@link #start}, which must reconnect from scratch, {@code
   * nativeSetPtzAndFixMode} is a plain passthrough to
   * MediaSDK_SetPtzAndFixMode and works on a live port. Used when the user
   * changes mount type/display mode after the stream is already up, since
   * restarting the whole RTSP connection on every such tap would be slow
   * and risks retriggering the connection issues this pipeline already had
   * to work around once. No-op if the stream hasn't started yet -- the
   * desired mode should instead be passed to {@link #start}'s initial
   * ptzMode/fixMode in that case.
   */
  public synchronized boolean setPtzAndFixMode(int ptzMode, int fixMode) {
    if (port == -1) return false;
    int rc = nativeSetPtzAndFixMode(port, ptzMode, fixMode);
    if (rc != 0) {
      Log.e(TAG, "MediaSDK_SetPtzAndFixMode failed, rc=" + rc);
    }
    return rc == 0;
  }

  public synchronized void stop() {
    if (port == -1) return;
    nativeStopStream(port);
    nativeDestroyPort(port);
    port = -1;
  }

  /**
   * Creates a Flutter Texture backed by a WebRTC SurfaceTextureRenderer
   * and returns its textureId. Must be called on the platform (UI)
   * thread -- SurfaceTextureRenderer.init() asserts this. Safe to call
   * before start(); frames simply have nowhere to go until this exists.
   */
  public static synchronized long createTexture(TextureRegistry textures) {
    disposeTexture();
    TextureRegistry.SurfaceProducer producer = textures.createSurfaceProducer();
    surfaceTextureRenderer = new SurfaceTextureRenderer("MediaSdkFisheye");
    surfaceTextureRenderer.init(EglUtils.getRootEglBaseContext(), null);
    surfaceTextureRenderer.surfaceCreated(producer);
    return producer.id();
  }

  public static synchronized void disposeTexture() {
    if (surfaceTextureRenderer == null) return;
    surfaceTextureRenderer.release();
    surfaceTextureRenderer = null;
  }

  /**
   * Called from mediasdk_jni.cpp on the SDK's own decode callback thread
   * for every corrected YUV420 frame. {@code dataY}/{@code dataU}/
   * {@code dataV} are direct ByteBuffers that merely view MediaSDK's own
   * native memory for the duration of this call -- they are copied here,
   * never retained past this method returning, matching the SDK's own
   * "process promptly" contract for this callback.
   *
   * Always fires the FlutterWebRTC event (see FlutterWebRTCPlugin
   * .eventSink) so frame arrival is observable from Dart even with no
   * texture attached; only renders if createTexture() was called first.
   */
  // DIAGNOSTIC: real-device testing (2026-10-13) showed the on-screen frame
  // counter freeze for minutes at a time while MediaSDK's own native decode
  // log (ProcessVideo/decodequeue.cpp) kept running at a rock-steady rate
  // throughout -- i.e. the native decode/network layer was NOT stalled, so
  // whatever blocks must be downstream of this callback. Timing each phase
  // below (with a loud WARN whenever one phase alone takes >200ms) is meant
  // to catch exactly which call -- the eventSink hop, the CPU-side plane
  // copy, or the SurfaceTextureRenderer/GL path -- is where a freeze
  // actually happens next time one is reproduced.
  private static final long SLOW_FRAME_THRESHOLD_MS = 200;

  // DIAGNOSTIC: checking whether MediaSDK_SetPtzAndFixMode actually changes
  // what this callback delivers -- the on-screen picture looked identical
  // across every PTZ mode tried (original/360+1PTZ/pano180) during manual
  // testing on 2026-10-14. Logs whenever width/height changes (a PTZ/pano
  // layout should be a different frame shape than the raw circular image)
  // and periodically logs the luma at frame center vs. the four corners --
  // a real fisheye circle is bright in the middle of the frame and black in
  // the corners (outside the lens' circular image), so if correction were
  // actually being applied those four corner samples should stop being
  // near-zero once a pano/PTZ mode unwraps the circle to fill the frame.
  private static int sLastWidth = -1;
  private static int sLastHeight = -1;

  private static void logFrameShapeDiagnostic(int width, int height, long frameCount,
                                               ByteBuffer dataY, int strideY) {
    if (width != sLastWidth || height != sLastHeight) {
      Log.w(TAG, "FRAME SHAPE CHANGED frameCount=" + frameCount
          + " " + sLastWidth + "x" + sLastHeight + " -> " + width + "x" + height);
      sLastWidth = width;
      sLastHeight = height;
    }
    if (frameCount % 50 != 0) return;
    int cx = width / 2;
    int cy = height / 2;
    int edge = Math.max(1, Math.min(width, height) / 20);
    int centerLuma = dataY.get(cy * strideY + cx) & 0xFF;
    int topLeftLuma = dataY.get(edge * strideY + edge) & 0xFF;
    int topRightLuma = dataY.get(edge * strideY + (width - edge)) & 0xFF;
    int bottomLeftLuma = dataY.get((height - edge) * strideY + edge) & 0xFF;
    int bottomRightLuma = dataY.get((height - edge) * strideY + (width - edge)) & 0xFF;
    Log.w(TAG, "FRAME LUMA frameCount=" + frameCount + " " + width + "x" + height
        + " center=" + centerLuma + " corners=[" + topLeftLuma + "," + topRightLuma + ","
        + bottomLeftLuma + "," + bottomRightLuma + "]");
  }

  private static void onFrame(int port, int width, int height, int dataType, long frameCount,
                               ByteBuffer dataY, int strideY, ByteBuffer dataU, int strideU,
                               ByteBuffer dataV, int strideV) {
    long tEntry = android.os.SystemClock.elapsedRealtime();
    logFrameShapeDiagnostic(width, height, frameCount, dataY, strideY);

    if (FlutterWebRTCPlugin.eventSink != null) {
      ConstraintsMap params = new ConstraintsMap();
      params.putString("event", "onFisheyeMediaSdkFrame");
      params.putInt("port", port);
      params.putInt("width", width);
      params.putInt("height", height);
      params.putInt("dataType", dataType);
      params.putLong("frameCount", frameCount);
      FlutterWebRTCPlugin.eventSink.success(params.toMap());
    }
    long tAfterEventSink = android.os.SystemClock.elapsedRealtime();

    SurfaceTextureRenderer renderer = surfaceTextureRenderer;
    if (renderer == null) {
      logFrameTiming(frameCount, tEntry, tAfterEventSink, tAfterEventSink, tAfterEventSink);
      return;
    }
    // MEDIASDK_VIDEO_RAW_YUV420 (dataType 0) is the only format this
    // path understands; matches what fisheyeMediaSdkStart always
    // requests (see MediaSdkFisheyePlayer.h's Android counterpart doc).
    if (dataType != 0) return;

    int chromaWidth = (width + 1) / 2;
    int chromaHeight = (height + 1) / 2;
    JavaI420Buffer buffer = JavaI420Buffer.allocate(width, height);
    copyPlane(dataY, strideY, buffer.getDataY(), buffer.getStrideY(), width, height);
    copyPlane(dataU, strideU, buffer.getDataU(), buffer.getStrideU(), chromaWidth, chromaHeight);
    copyPlane(dataV, strideV, buffer.getDataV(), buffer.getStrideV(), chromaWidth, chromaHeight);
    long tAfterCopy = android.os.SystemClock.elapsedRealtime();

    VideoFrame frame = new VideoFrame(buffer, 0, System.nanoTime());
    renderer.onFrame(frame);
    frame.release();
    long tAfterRender = android.os.SystemClock.elapsedRealtime();

    logFrameTiming(frameCount, tEntry, tAfterEventSink, tAfterCopy, tAfterRender);
  }

  /**
   * Logs a WARN line (so it survives casual grepping) whenever any single
   * phase of onFrame() takes longer than {@link #SLOW_FRAME_THRESHOLD_MS},
   * plus a cheap periodic INFO heartbeat so the timeline can be reconstructed
   * even if no phase ever crosses the threshold.
   */
  private static void logFrameTiming(long frameCount, long tEntry, long tAfterEventSink,
                                       long tAfterCopy, long tAfterRender) {
    long eventSinkMs = tAfterEventSink - tEntry;
    long copyMs = tAfterCopy - tAfterEventSink;
    long renderMs = tAfterRender - tAfterCopy;
    long totalMs = tAfterRender - tEntry;
    if (totalMs >= SLOW_FRAME_THRESHOLD_MS) {
      Log.w(TAG, "SLOW FRAME frameCount=" + frameCount + " totalMs=" + totalMs
          + " eventSinkMs=" + eventSinkMs + " copyMs=" + copyMs + " renderMs=" + renderMs);
    } else if (frameCount % 100 == 0) {
      Log.i(TAG, "frame timing heartbeat frameCount=" + frameCount + " totalMs=" + totalMs
          + " eventSinkMs=" + eventSinkMs + " copyMs=" + copyMs + " renderMs=" + renderMs);
    }
  }

  /** Row-by-row copy so source/destination strides don't need to match. */
  private static void copyPlane(ByteBuffer src, int srcStride, ByteBuffer dst, int dstStride,
                                 int width, int height) {
    byte[] row = new byte[width];
    for (int y = 0; y < height; y++) {
      src.position(y * srcStride);
      src.get(row, 0, width);
      dst.position(y * dstStride);
      dst.put(row, 0, width);
    }
  }
}
