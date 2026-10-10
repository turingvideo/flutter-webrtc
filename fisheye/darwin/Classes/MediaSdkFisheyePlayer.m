#import "MediaSdkFisheyePlayer.h"

// Quote-style + an explicit header search path (see
// ../../ios/flutter_webrtc.podspec's USER_HEADER_SEARCH_PATHS) rather than
// <libMediaSDK/media_sdk_func.h> framework-style angle brackets -- the
// latter didn't resolve on a real device build even with libMediaSDK.framework
// declared as a vendored_framework (presumably needs a module map this
// plain Headers-folder framework bundle doesn't have). The vendor's C
// headers are otherwise identical whether read from inside the framework
// bundle or the flat fisheye/ios/vendor/include copy (see
// fisheye/android/vendor/include's copy, diffed byte-identical when this
// SDK drop was imported).
#import "media_sdk_func.h"
#import <string.h>
#import <os/lock.h>
#import <arpa/inet.h>
#import <netdb.h>

// MEDIASDK_VIDEO_CB_TYPE_E's "processed/corrected data" value -- see
// fisheye/README.md step 4 and mediasdk_jni.cpp's matching constant on
// Android. Not spelled out as a symbol under this exact name in this SDK
// drop, hardcoded here exactly as the vendor's own sample does (`= 1`).
static const ULONG_32 kMediaSdkVideoCbProcessedData = 1;

// Pulls "user" and "pass" out of an rtsp://user:pass@host:port/path URL,
// if present. Mirrors ExtractUrlCredentials in mediasdk_jni.cpp exactly
// -- see that function's doc for why this is called explicitly even
// though the URL already carries the credentials.
static BOOL ExtractUrlCredentials(NSString *url, NSString **user, NSString **pass) {
  NSRange schemeRange = [url rangeOfString:@"://"];
  if (schemeRange.location == NSNotFound) return NO;
  NSUInteger authorityStart = schemeRange.location + schemeRange.length;
  NSRange atRange = [url rangeOfString:@"@" options:0
                                  range:NSMakeRange(authorityStart, url.length - authorityStart)];
  if (atRange.location == NSNotFound) return NO;
  NSString *credentials = [url substringWithRange:NSMakeRange(authorityStart,
                                                                atRange.location - authorityStart)];
  NSRange colonRange = [credentials rangeOfString:@":"];
  if (colonRange.location == NSNotFound) return NO;
  *user = [credentials substringToIndex:colonRange.location];
  *pass = [credentials substringFromIndex:colonRange.location + colonRange.length];
  return YES;
}

// Pulls "host" and "port" (0 if absent) out of an
// rtsp://[user:pass@]host[:port]/path URL. Mirrors ExtractUrlHostPort in
// mediasdk_jni.cpp exactly -- see that function's doc for why
// pcServerIp/usServerPort are filled even for a plain local connection.
static BOOL ExtractUrlHostPort(NSString *url, NSString **host, int *port) {
  NSRange schemeRange = [url rangeOfString:@"://"];
  if (schemeRange.location == NSNotFound) return NO;
  NSUInteger authorityStart = schemeRange.location + schemeRange.length;
  NSRange atRange = [url rangeOfString:@"@" options:0
                                  range:NSMakeRange(authorityStart, url.length - authorityStart)];
  NSUInteger hostStart =
      (atRange.location != NSNotFound) ? atRange.location + atRange.length : authorityStart;
  NSRange pathRange = [url rangeOfString:@"/" options:0
                                    range:NSMakeRange(hostStart, url.length - hostStart)];
  NSString *authority = (pathRange.location != NSNotFound)
      ? [url substringWithRange:NSMakeRange(hostStart, pathRange.location - hostStart)]
      : [url substringFromIndex:hostStart];
  NSRange colonRange = [authority rangeOfString:@":"];
  if (colonRange.location == NSNotFound) {
    *host = authority;
    *port = 0;
  } else {
    *host = [authority substringToIndex:colonRange.location];
    *port = [[authority substringFromIndex:colonRange.location + colonRange.length] intValue];
  }
  return (*host).length > 0;
}

// pcServerIp is named (and, per a real-device Android test against a
// hostname-based cloud RTSP URL, apparently treated) as a literal
// dotted-decimal IPv4 address -- passing a hostname straight through
// connects fine for a LAN-camera URL (whose "host" already is an IP) but
// fails for a real domain like dev-srs.turingvideo.com with ENETUNREACH
// deep in the vendor SDK's own socket layer, since it never resolves the
// string itself. This does that resolution ourselves via the platform's
// regular DNS. Mirrors ResolveHostToIp in mediasdk_jni.cpp exactly. A
// literal-IP host round-trips through getaddrinfo/inet_ntop unchanged, so
// this is safe to call unconditionally rather than only for hostnames.
static BOOL ResolveHostToIp(NSString *host, NSString **ip) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  struct addrinfo *result = NULL;
  int rc = getaddrinfo(host.UTF8String, NULL, &hints, &result);
  if (rc != 0 || result == NULL) {
    NSLog(@"[MediaSdkFisheyePlayer] getaddrinfo(%@) failed: %s", host, gai_strerror(rc));
    return NO;
  }
  char buf[INET_ADDRSTRLEN] = {0};
  struct sockaddr_in *addr = (struct sockaddr_in *)result->ai_addr;
  const char *formatted = inet_ntop(AF_INET, &addr->sin_addr, buf, sizeof(buf));
  freeaddrinfo(result);
  if (formatted == NULL) return NO;
  *ip = [NSString stringWithUTF8String:buf];
  return YES;
}

static inline uint8_t ClampU8(int value) {
  return (uint8_t)(value < 0 ? 0 : (value > 255 ? 255 : value));
}

// BT.601 full-range YUV -> BGRA, matching the same math as
// BLIT_YUV_FRAGMENT_SHADER (DewarpGlDrawer.java) and
// kPanoramaWarpKernelSource, just scaled for 8-bit ints instead of GLSL
// floats. width/height are the luma plane's; u/v are 4:2:0 subsampled
// (half resolution, rounded up) with their own strides.
static void ConvertYuv420ToBgra(const uint8_t *y, int strideY, const uint8_t *u, int strideU,
                                 const uint8_t *v, int strideV, int width, int height,
                                 uint8_t *dst, size_t dstStride) {
  for (int row = 0; row < height; row++) {
    const uint8_t *yRow = y + (size_t)row * strideY;
    const uint8_t *uRow = u + (size_t)(row / 2) * strideU;
    const uint8_t *vRow = v + (size_t)(row / 2) * strideV;
    uint8_t *dstRow = dst + (size_t)row * dstStride;
    for (int col = 0; col < width; col++) {
      int Y = yRow[col];
      int U = uRow[col / 2] - 128;
      int V = vRow[col / 2] - 128;
      int r = Y + (1403 * V) / 1000;
      int g = Y - (344 * U + 714 * V) / 1000;
      int b = Y + (1770 * U) / 1000;
      uint8_t *px = dstRow + (size_t)col * 4;
      px[0] = ClampU8(b);
      px[1] = ClampU8(g);
      px[2] = ClampU8(r);
      px[3] = 255;
    }
  }
}

@implementation MediaSdkFisheyePlayer {
  ULONG_32 _port;
  BOOL _hasPort;
  unsigned long long _frameCount;

  id<FlutterTextureRegistry> _textureRegistry;
  int64_t _textureId;
  CVPixelBufferRef _pixelBufferRef;
  os_unfair_lock _pixelLock;
}

- (instancetype)init {
  self = [super init];
  if (self) {
    _pixelLock = OS_UNFAIR_LOCK_INIT;
    _textureId = -1;
  }
  return self;
}

static BOOL sSdkInited = NO;

// Forwards MediaSDK's internal debug log via NSLog. Per media_sdk_func.h's
// doc on MediaSDK_SetWriteLogCB: "if the caller doesn't register this
// callback, the SDK records no internal log information at all" -- mirrors
// MediaSdkWriteLogCallback in mediasdk_jni.cpp exactly.
static void MediaSdkWriteLogCallback(ULONG_32 logLevel, ULONG_32 errCode, ULONG_32 cmdModuleId,
                                      const CHAR *fileName, ULONG_32 fileLine,
                                      const CHAR *logDescription) {
  NSLog(@"[MediaSdkFisheyePlayer][MediaSDK-CB] level=%u err=0x%x module=%u %s:%u %s",
        (unsigned int)logLevel, (unsigned int)errCode, (unsigned int)cmdModuleId,
        fileName ? fileName : "", (unsigned int)fileLine, logDescription ? logDescription : "");
}

+ (BOOL)ensureInited {
  if (sSdkInited) {
    return YES;
  }
  // Must be called before any other MediaSDK function per its own doc.
  ULONG_32 logRc = MediaSDK_SetWriteLogCB(MediaSdkWriteLogCallback, MEDIASDK_LOG_DEBUG);
  NSLog(@"[MediaSdkFisheyePlayer] MediaSDK_SetWriteLogCB rc=%u", (unsigned int)logRc);
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
  if (sink) {
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

  if (_textureId < 0 || _textureRegistry == nil) {
    return;
  }
  // MEDIASDK_VIDEO_RAW_YUV420 (dataType 0) is the only format this path
  // understands; matches what startWithRtspUrl:... always gets (see
  // fisheyeMediaSdkStart's Android counterpart doc).
  if (frame->eDataType != 0) {
    return;
  }

  int width = (int)frame->ulPicWidth;
  int height = (int)frame->ulPicHeight;

  CVPixelBufferRef newBuffer = NULL;
  NSDictionary *attrs = @{(id)kCVPixelBufferIOSurfacePropertiesKey : @{}};
  CVPixelBufferCreate(kCFAllocatorDefault, width, height, kCVPixelFormatType_32BGRA,
                      (__bridge CFDictionaryRef)attrs, &newBuffer);
  if (newBuffer == NULL) {
    return;
  }

  CVPixelBufferLockBaseAddress(newBuffer, 0);
  uint8_t *dst = (uint8_t *)CVPixelBufferGetBaseAddress(newBuffer);
  size_t dstStride = CVPixelBufferGetBytesPerRow(newBuffer);
  ConvertYuv420ToBgra(frame->pucData[0], frame->ulLineSize[0], frame->pucData[1],
                      frame->ulLineSize[1], frame->pucData[2], frame->ulLineSize[2], width, height,
                      dst, dstStride);
  CVPixelBufferUnlockBaseAddress(newBuffer, 0);

  os_unfair_lock_lock(&_pixelLock);
  CVPixelBufferRef old = _pixelBufferRef;
  _pixelBufferRef = newBuffer;
  os_unfair_lock_unlock(&_pixelLock);
  if (old != NULL) {
    CVBufferRelease(old);
  }

  id<FlutterTextureRegistry> registry = _textureRegistry;
  int64_t textureId = _textureId;
  dispatch_async(dispatch_get_main_queue(), ^{
    [registry textureFrameAvailable:textureId];
  });
}

- (CVPixelBufferRef)copyPixelBuffer {
  os_unfair_lock_lock(&_pixelLock);
  CVPixelBufferRef buffer = _pixelBufferRef != NULL ? CVBufferRetain(_pixelBufferRef) : NULL;
  os_unfair_lock_unlock(&_pixelLock);
  return buffer;
}

- (int64_t)createTextureWithRegistry:(id<FlutterTextureRegistry>)registry {
  [self disposeTexture];
  _textureRegistry = registry;
  _textureId = [registry registerTexture:self];
  return _textureId;
}

- (void)disposeTexture {
  if (_textureId >= 0 && _textureRegistry != nil) {
    [_textureRegistry unregisterTexture:_textureId];
  }
  _textureId = -1;
  _textureRegistry = nil;

  os_unfair_lock_lock(&_pixelLock);
  CVPixelBufferRef old = _pixelBufferRef;
  _pixelBufferRef = NULL;
  os_unfair_lock_unlock(&_pixelLock);
  if (old != NULL) {
    CVBufferRelease(old);
  }
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

  NSString *user, *pass;
  if (ExtractUrlCredentials(rtspUrl, &user, &pass)) {
    ULONG_32 credRc =
        MediaSDK_SetUserPassword(_port, (CHAR *)[user UTF8String], (CHAR *)[pass UTF8String]);
    NSLog(@"[MediaSdkFisheyePlayer] MediaSDK_SetUserPassword user=%@ rc=%u", user,
          (unsigned int)credRc);
  }

  MEDIASDK_RTSP_REALPLAY_S stRealPlay;
  memset(&stRealPlay, 0, sizeof(stRealPlay));
  stRealPlay.enRtspType = MEDIASDK_RTSP_RTP_TCP;
  stRealPlay.enRtspExecuteType = MEDIASDK_RTSP_EXECUTE_TYPE_STANDARD;
  stRealPlay.enStreamMode = MEDIASDK_RTSP_MODE_ONLY_VIDEO;
  stRealPlay.pcURL = (CHAR *)[rtspUrl UTF8String];
  stRealPlay.pWnd = NULL;  // no display window -- see class doc.

  NSString *host;
  int urlPort = 0;
  // See ExtractUrlHostPort's doc -- a real-device test on Android showed
  // this build requires pcServerIp/usServerPort even for a plain local
  // connection, despite the header doc implying they're only for
  // cross-network/P2P use. Untested in isolation on iOS specifically,
  // but applying the same fix here keeps the two platforms in sync
  // rather than waiting to hit the identical failure separately.
  if (ExtractUrlHostPort(rtspUrl, &host, &urlPort)) {
    NSString *resolvedIp;
    if (ResolveHostToIp(host, &resolvedIp)) {
      NSLog(@"[MediaSdkFisheyePlayer] Resolved host=%@ to ip=%@", host, resolvedIp);
      host = resolvedIp;
    } else {
      NSLog(@"[MediaSdkFisheyePlayer] Failed to resolve host=%@, passing it through unresolved",
            host);
    }
    stRealPlay.pcServerIp = (CHAR *)[host UTF8String];
    stRealPlay.usServerPort = (USHORT)(urlPort != 0 ? urlPort : 554);
  }

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
