// JNI glue between MediaSdkFisheyePlayer.java and the vendor MediaSDK C
// API (fisheye/android/vendor/include/media_sdk_func.h). Mirrors the
// reference call sequence in fisheye/README.md exactly; see that file
// and MediaSdkFisheyePlayer.java's class doc for the bigger picture.

#include <jni.h>
#include <android/log.h>
#include <string.h>

#include "media_sdk_func.h"

#define LOG_TAG "fisheye_mediasdk_jni"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace {

JavaVM* g_vm = nullptr;
jclass g_playerClass = nullptr;      // global ref to MediaSdkFisheyePlayer
jmethodID g_onFrameMethod = nullptr; // static void onFrame(int,int,int,int,long)

// MEDIASDK_VIDEO_CB_TYPE_E's "processed/corrected data" value -- see
// fisheye/README.md step 4. Not spelled out as a symbol in the header
// under that exact name in this SDK drop, so it's hardcoded here exactly
// as the vendor's own sample does (`= 1`).
constexpr ULONG_32 kMediaSdkVideoCbProcessedData = 1;

JNIEnv* AttachCurrentThreadIfNeeded(bool* didAttach) {
  JNIEnv* env = nullptr;
  jint getEnvResult = g_vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
  if (getEnvResult == JNI_OK) {
    *didAttach = false;
    return env;
  }
  if (g_vm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
    LOGE("AttachCurrentThread failed on MediaSDK callback thread");
    *didAttach = false;
    return nullptr;
  }
  *didAttach = true;
  return env;
}

// Matches MEDIASDK_DECODE_VIDEO_DATA_CALLBACK_PF exactly (media_sdk_com_def.h).
// Invoked on MediaSDK's own internal decode thread, not a JNI-attached one.
void STDCALL OnDecodeVideoData(ULONG_32 ulPort,
                                const MEDIASDK_DECODE_VIDEO_RAW_S* pDecodeVideoData,
                                VOID* pUserParam,
                                LONG_REAL lReserved) {
  (void)pUserParam;
  (void)lReserved;
  if (pDecodeVideoData == nullptr) return;

  static ULONG_32 frameCounts[64] = {0};
  ULONG_32 frameCount = 0;
  if (ulPort < 64) {
    frameCount = ++frameCounts[ulPort];
  }

  bool didAttach = false;
  JNIEnv* env = AttachCurrentThreadIfNeeded(&didAttach);
  if (env == nullptr) return;

  env->CallStaticVoidMethod(g_playerClass, g_onFrameMethod,
                             static_cast<jint>(ulPort),
                             static_cast<jint>(pDecodeVideoData->ulPicWidth),
                             static_cast<jint>(pDecodeVideoData->ulPicHeight),
                             static_cast<jint>(pDecodeVideoData->eDataType),
                             static_cast<jlong>(frameCount));

  if (didAttach) {
    g_vm->DetachCurrentThread();
  }
}

}  // namespace

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* /*reserved*/) {
  g_vm = vm;
  JNIEnv* env = nullptr;
  if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return JNI_ERR;
  }
  jclass localClass = env->FindClass("com/cloudwebrtc/webrtc/fisheye/MediaSdkFisheyePlayer");
  if (localClass == nullptr) {
    LOGE("Could not find MediaSdkFisheyePlayer class");
    return JNI_ERR;
  }
  g_playerClass = reinterpret_cast<jclass>(env->NewGlobalRef(localClass));
  g_onFrameMethod = env->GetStaticMethodID(g_playerClass, "onFrame", "(IIIIJ)V");
  if (g_onFrameMethod == nullptr) {
    LOGE("Could not find MediaSdkFisheyePlayer.onFrame(IIIIJ)V");
    return JNI_ERR;
  }
  return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* /*reserved*/) {
  JNIEnv* env = nullptr;
  if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK && g_playerClass != nullptr) {
    env->DeleteGlobalRef(g_playerClass);
  }
  g_playerClass = nullptr;
  g_onFrameMethod = nullptr;
  g_vm = nullptr;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_cloudwebrtc_webrtc_fisheye_MediaSdkFisheyePlayer_nativeInit(JNIEnv*, jclass) {
  ULONG_32 rc = MediaSDK_Init();
  LOGI("MediaSDK_Init rc=%u", rc);
  return static_cast<jint>(rc);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_cloudwebrtc_webrtc_fisheye_MediaSdkFisheyePlayer_nativeCleanup(JNIEnv*, jclass) {
  ULONG_32 rc = MediaSDK_Cleanup();
  LOGI("MediaSDK_Cleanup rc=%u", rc);
  return static_cast<jint>(rc);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_cloudwebrtc_webrtc_fisheye_MediaSdkFisheyePlayer_nativeCreatePort(JNIEnv* env, jclass,
                                                                            jintArray outPort) {
  ULONG_32 ulPort = 0;
  ULONG_32 rc = MediaSDK_CreatPortRes(&ulPort);
  LOGI("MediaSDK_CreatPortRes rc=%u port=%u", rc, ulPort);
  jint portValue = static_cast<jint>(ulPort);
  env->SetIntArrayRegion(outPort, 0, 1, &portValue);
  return static_cast<jint>(rc);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_cloudwebrtc_webrtc_fisheye_MediaSdkFisheyePlayer_nativeDestroyPort(JNIEnv*, jclass,
                                                                             jint port) {
  ULONG_32 rc = MediaSDK_DestroyPortRes(static_cast<ULONG_32>(port));
  LOGI("MediaSDK_DestroyPortRes port=%d rc=%u", port, rc);
  return static_cast<jint>(rc);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_cloudwebrtc_webrtc_fisheye_MediaSdkFisheyePlayer_nativeSetDecodeVideoDataCB(JNIEnv*,
                                                                                      jclass,
                                                                                      jint port) {
  ULONG_32 rc = MediaSDK_SetDecodeVideoDataCB(static_cast<ULONG_32>(port), OnDecodeVideoData,
                                               BOOL_TRUE, nullptr);
  if (rc != ERR_MEDIASDK_SUCCEED) {
    LOGE("MediaSDK_SetDecodeVideoDataCB failed rc=%u", rc);
    return static_cast<jint>(rc);
  }
  // Step 4 in fisheye/README.md -- switch the callback to deliver
  // fisheye-corrected data instead of the raw decoded circle.
  rc = MediaSDK_SetVideoCallBackMode(static_cast<ULONG_32>(port), kMediaSdkVideoCbProcessedData);
  LOGI("MediaSDK_SetVideoCallBackMode rc=%u", rc);
  return static_cast<jint>(rc);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_cloudwebrtc_webrtc_fisheye_MediaSdkFisheyePlayer_nativeStartRtspStream(JNIEnv* env,
                                                                                 jclass,
                                                                                 jint port,
                                                                                 jstring rtspUrl) {
  const char* url = env->GetStringUTFChars(rtspUrl, nullptr);

  MEDIASDK_RTSP_REALPLAY_S stRealPlay;
  memset(&stRealPlay, 0, sizeof(stRealPlay));
  stRealPlay.enRtspType = MEDIASDK_RTSP_RTP_TCP;
  stRealPlay.enRtspExecuteType = MEDIASDK_RTSP_EXECUTE_TYPE_STANDARD;
  stRealPlay.enStreamMode = MEDIASDK_RTSP_MODE_ONLY_VIDEO;
  stRealPlay.pcURL = const_cast<char*>(url);
  stRealPlay.pWnd = nullptr;  // no display window -- see MediaSdkFisheyePlayer's doc.

  ULONG_32 rc = MediaSDK_StartStream(static_cast<ULONG_32>(port), MEDIASDK_STREAM_RTSP_REALPLAY,
                                      &stRealPlay, BOOL_FALSE);
  LOGI("MediaSDK_StartStream port=%d url=%s rc=%u", port, url, rc);

  env->ReleaseStringUTFChars(rtspUrl, url);
  return static_cast<jint>(rc);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_cloudwebrtc_webrtc_fisheye_MediaSdkFisheyePlayer_nativeStopStream(JNIEnv*, jclass,
                                                                            jint port) {
  ULONG_32 rc = MediaSDK_StopStream(static_cast<ULONG_32>(port));
  LOGI("MediaSDK_StopStream port=%d rc=%u", port, rc);
  return static_cast<jint>(rc);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_cloudwebrtc_webrtc_fisheye_MediaSdkFisheyePlayer_nativeSetPtzAndFixMode(JNIEnv*, jclass,
                                                                                  jint port,
                                                                                  jint ptzMode,
                                                                                  jint fixMode) {
  ULONG_32 rc = MediaSDK_SetPtzAndFixMode(static_cast<ULONG_32>(port),
                                           static_cast<ULONG_32>(ptzMode),
                                           static_cast<ULONG_32>(fixMode));
  LOGI("MediaSDK_SetPtzAndFixMode port=%d ptzMode=%d fixMode=%d rc=%u", port, ptzMode, fixMode, rc);
  return static_cast<jint>(rc);
}
