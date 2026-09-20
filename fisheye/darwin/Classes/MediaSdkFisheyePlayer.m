#import "MediaSdkFisheyePlayer.h"

#import <libMediaSDK/media_sdk_func.h>
#import <string.h>

// MEDIASDK_VIDEO_CB_TYPE_E's "processed/corrected data" value -- see
// fisheye/README.md step 4 and mediasdk_jni.cpp's matching constant on
// Android. Not spelled out as a symbol under this exact name in this SDK
// drop, hardcoded here exactly as the vendor's own sample does (`= 1`).
static const ULONG_32 kMediaSdkVideoCbProcessedData = 1;

@implementation MediaSdkFisheyePlayer {
  ULONG_32 _port;
  BOOL _hasPort;
  unsigned long long _frameCount;
}

static BOOL sSdkInited = NO;

+ (BOOL)ensureInited {
  if (sSdkInited) {
    return YES;
  }
  ULONG_32 rc = MediaSDK_Init();
  sSdkInited = (rc == ERR_MEDIASDK_SUCCEED);
  if (!sSdkInited) {
    NSLog(@"[MediaSdkFisheyePlayer] MediaSDK_Init failed rc=%u", (unsigned int)rc);
  }
  return sSdkInited;
}

// Matches MEDIASDK_DECODE_VIDEO_DATA_CALLBACK_PF exactly (media_sdk_com_def.h).
static void OnDecodeVideoData(ULONG_32 ulPort,
                               const MEDIASDK_DECODE_VIDEO_RAW_S *pDecodeVideoData,
                               VOID *pUserParam,
                               LONG_REAL lReserved) {
  (void)lReserved;
  if (pDecodeVideoData == NULL || pUserParam == NULL) {
    return;
  }
  MediaSdkFisheyePlayer *player = (__bridge MediaSdkFisheyePlayer *)pUserParam;
  [player handleDecodedFrame:pDecodeVideoData port:ulPort];
}

- (void)handleDecodedFrame:(const MEDIASDK_DECODE_VIDEO_RAW_S *)frame port:(ULONG_32)port {
  _frameCount++;
  FlutterEventSink sink = self.eventSink;
  if (!sink) {
    return;
  }
  NSDictionary *event = @{
    @"event" : @"onFisheyeMediaSdkFrame",
    @"port" : @(port),
    @"width" : @(frame->ulPicWidth),
    @"height" : @(frame->ulPicHeight),
    @"dataType" : @(frame->eDataType),
    @"frameCount" : @(_frameCount),
  };
  dispatch_async(dispatch_get_main_queue(), ^{
    sink(event);
  });
}

- (BOOL)startWithRtspUrl:(NSString *)rtspUrl ptzMode:(NSUInteger)ptzMode fixMode:(NSUInteger)fixMode {
  if (![MediaSdkFisheyePlayer ensureInited]) {
    return NO;
  }
  if (_hasPort) {
    NSLog(@"[MediaSdkFisheyePlayer] start called while already running on port %u -- stopping first",
          (unsigned int)_port);
    [self stop];
  }

  ULONG_32 port = 0;
  ULONG_32 rc = MediaSDK_CreatPortRes(&port);
  if (rc != ERR_MEDIASDK_SUCCEED) {
    NSLog(@"[MediaSdkFisheyePlayer] MediaSDK_CreatPortRes failed rc=%u", (unsigned int)rc);
    return NO;
  }
  _port = port;
  _hasPort = YES;
  _frameCount = 0;

  rc = MediaSDK_SetDecodeVideoDataCB(_port, OnDecodeVideoData, BOOL_TRUE, (__bridge VOID *)self);
  if (rc != ERR_MEDIASDK_SUCCEED) {
    NSLog(@"[MediaSdkFisheyePlayer] MediaSDK_SetDecodeVideoDataCB failed rc=%u", (unsigned int)rc);
    MediaSDK_DestroyPortRes(_port);
    _hasPort = NO;
    return NO;
  }

  // Step 4 in fisheye/README.md -- switch the callback to deliver
  // fisheye-corrected data instead of the raw decoded circle.
  rc = MediaSDK_SetVideoCallBackMode(_port, kMediaSdkVideoCbProcessedData);
  if (rc != ERR_MEDIASDK_SUCCEED) {
    NSLog(@"[MediaSdkFisheyePlayer] MediaSDK_SetVideoCallBackMode failed rc=%u", (unsigned int)rc);
    MediaSDK_DestroyPortRes(_port);
    _hasPort = NO;
    return NO;
  }

  MEDIASDK_RTSP_REALPLAY_S stRealPlay;
  memset(&stRealPlay, 0, sizeof(stRealPlay));
  stRealPlay.enRtspType = MEDIASDK_RTSP_RTP_TCP;
  stRealPlay.enRtspExecuteType = MEDIASDK_RTSP_EXECUTE_TYPE_STANDARD;
  stRealPlay.enStreamMode = MEDIASDK_RTSP_MODE_ONLY_VIDEO;
  stRealPlay.pcURL = (CHAR *)[rtspUrl UTF8String];
  stRealPlay.pWnd = NULL;  // no display window -- see class doc.

  rc = MediaSDK_StartStream(_port, MEDIASDK_STREAM_RTSP_REALPLAY, &stRealPlay, BOOL_FALSE);
  if (rc != ERR_MEDIASDK_SUCCEED) {
    NSLog(@"[MediaSdkFisheyePlayer] MediaSDK_StartStream failed rc=%u", (unsigned int)rc);
    MediaSDK_DestroyPortRes(_port);
    _hasPort = NO;
    return NO;
  }

  rc = MediaSDK_SetPtzAndFixMode(_port, (ULONG_32)ptzMode, (ULONG_32)fixMode);
  if (rc != ERR_MEDIASDK_SUCCEED) {
    // Not fatal to the stream itself -- leave it running so the raw
    // (uncorrected) circle is still watchable/debuggable, same reasoning
    // as the Android side.
    NSLog(@"[MediaSdkFisheyePlayer] MediaSDK_SetPtzAndFixMode failed rc=%u", (unsigned int)rc);
  }
  return YES;
}

- (void)stop {
  if (!_hasPort) {
    return;
  }
  MediaSDK_StopStream(_port);
  MediaSDK_DestroyPortRes(_port);
  _hasPort = NO;
}

@end
