package com.cloudwebrtc.webrtc.fisheye;

import android.util.Log;

import com.cloudwebrtc.webrtc.FlutterWebRTCPlugin;
import com.cloudwebrtc.webrtc.utils.ConstraintsMap;

/**
 * Thin JNI wrapper around the vendor MediaSDK (fisheye/android/vendor,
 * headers in fisheye/android/vendor/include) -- mirrors the reference C
 * call sequence from fisheye/README.md: Init -> CreatPortRes ->
 * SetDecodeVideoDataCB -> SetVideoCallBackMode(PROCESSED_DATA) ->
 * StartStream(RTSP realplay, pWnd=NULL) -> SetPtzAndFixMode -> ... ->
 * StopStream -> DestroyPortRes -> Cleanup. The actual calls live in the
 * JNI glue at fisheye/android/src/main/cpp/mediasdk_jni.cpp.
 *
 * This class only proves the pipeline runs end-to-end and delivers
 * corrected YUV frames -- it does not render anything to screen yet.
 * Frame arrival is reported via FlutterWebRTC's existing shared event
 * channel (see onFrame() below) purely so it's observable from Dart
 * without a display pipeline while that part is still unbuilt. Turning
 * those YUV planes into an actual on-screen Flutter Texture (GPU upload
 * + SurfaceTexture, matching how DewarpGlDrawer already handles WebRTC's
 * own decoded frames) is separate, not-yet-started work.
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
   * Called from mediasdk_jni.cpp on the SDK's own decode callback thread
   * for every corrected YUV frame. Forwarded as a normal FlutterWebRTC
   * event (see FlutterWebRTCPlugin.eventSink) so Dart can observe frames
   * actually arriving during this pipeline's bring-up, before any
   * rendering path exists.
   */
  private static void onFrame(int port, int width, int height, int dataType, long frameCount) {
    if (FlutterWebRTCPlugin.eventSink == null) return;
    ConstraintsMap params = new ConstraintsMap();
    params.putString("event", "onFisheyeMediaSdkFrame");
    params.putInt("port", port);
    params.putInt("width", width);
    params.putInt("height", height);
    params.putInt("dataType", dataType);
    params.putLong("frameCount", frameCount);
    FlutterWebRTCPlugin.eventSink.success(params.toMap());
  }
}
