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

    rc = nativeSetPtzAndFixMode(port, ptzMode, fixMode);
    if (rc != 0) {
      // Not fatal to the stream itself -- leave it running so the raw
      // (uncorrected) circle is still watchable/debuggable.
      Log.e(TAG, "MediaSDK_SetPtzAndFixMode failed, rc=" + rc);
    }
    return true;
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
  private static void onFrame(int port, int width, int height, int dataType, long frameCount,
                               ByteBuffer dataY, int strideY, ByteBuffer dataU, int strideU,
                               ByteBuffer dataV, int strideV) {
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

    SurfaceTextureRenderer renderer = surfaceTextureRenderer;
    if (renderer == null) return;
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

    VideoFrame frame = new VideoFrame(buffer, 0, System.nanoTime());
    renderer.onFrame(frame);
    frame.release();
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
