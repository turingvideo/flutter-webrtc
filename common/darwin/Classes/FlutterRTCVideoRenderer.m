#import "FlutterRTCVideoRenderer.h"

#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CGImage.h>
#import <WebRTC/RTCYUVHelper.h>
#import <WebRTC/RTCYUVPlanarBuffer.h>
#import <WebRTC/WebRTC.h>

#import <objc/runtime.h>

#import "FlutterWebRTCPlugin.h"
#import "RTCDewarpProcessor.h"
#import <os/lock.h>

@implementation FlutterRTCVideoRenderer {
  // Size of _pixelBufferRef: the raw decoded frame size when dewarp is
  // off, or the dewarped composite canvas size when it's on -- i.e.
  // whatever's actually displayed. Reported to Dart in place of the raw
  // decoder frame size (see -renderFrame:) so RTCVideoView's
  // aspect-ratio-driven layout stays correct.
  CGSize _frameSize;
  CGSize _renderSize;
  CVPixelBufferRef _pixelBufferRef;
  // Raw decoded frame size/buffer, only allocated while _dewarpConfig is
  // non-nil: -renderFrame: writes the upright decoded frame here first,
  // then _dewarpProcessor reads from it and writes the composite tile
  // grid into _pixelBufferRef.
  CGSize _rawFrameSize;
  CVPixelBufferRef _rawPixelBufferRef;
  RTCDewarpConfig* _dewarpConfig;
  RTCDewarpProcessor* _dewarpProcessor;
  RTCVideoRotation _rotation;
  FlutterEventChannel* _eventChannel;
  bool _isFirstFrameRendered;
  bool _frameAvailable;
  os_unfair_lock _lock;
}

@synthesize textureId = _textureId;
@synthesize registry = _registry;
@synthesize eventSink = _eventSink;
@synthesize videoTrack = _videoTrack;

- (instancetype)initWithTextureRegistry:(id<FlutterTextureRegistry>)registry
                              messenger:(NSObject<FlutterBinaryMessenger>*)messenger {
  self = [super init];
  if (self) {
    _lock = OS_UNFAIR_LOCK_INIT;
    _isFirstFrameRendered = false;
    _frameAvailable = false;
    _frameSize = CGSizeZero;
    _renderSize = CGSizeZero;
    _rotation = -1;
    _registry = registry;
    _pixelBufferRef = nil;
    _eventSink = nil;
    _rotation = -1;
    _textureId = [registry registerTexture:self];
    /*Create Event Channel.*/
    _eventChannel = [FlutterEventChannel
        eventChannelWithName:[NSString stringWithFormat:@"FlutterWebRTC/Texture%lld", _textureId]
             binaryMessenger:messenger];
    [_eventChannel setStreamHandler:self];
  }
  return self;
}

- (CVPixelBufferRef)copyPixelBuffer {
  CVPixelBufferRef buffer = nil;
  os_unfair_lock_lock(&_lock);
  if (_pixelBufferRef != nil && _frameAvailable) {
    buffer = CVBufferRetain(_pixelBufferRef);
    _frameAvailable = false;
  }
  os_unfair_lock_unlock(&_lock);
  return buffer;
}

- (void)dispose {
  os_unfair_lock_lock(&_lock);
  [_registry unregisterTexture:_textureId];
  _textureId = -1;
  if (_pixelBufferRef) {
    CVBufferRelease(_pixelBufferRef);
    _pixelBufferRef = nil;
  }
  if (_rawPixelBufferRef) {
    CVBufferRelease(_rawPixelBufferRef);
    _rawPixelBufferRef = nil;
  }
  _frameAvailable = false;
  os_unfair_lock_unlock(&_lock);
}

- (void)setVideoTrack:(RTCVideoTrack*)videoTrack {
  RTCVideoTrack* oldValue = self.videoTrack;
  if (oldValue != videoTrack) {
    os_unfair_lock_lock(&_lock);
    _videoTrack = videoTrack;
    os_unfair_lock_unlock(&_lock);
    _isFirstFrameRendered = false;
    if (oldValue) {
      [oldValue removeRenderer:self];
    }
    _frameSize = CGSizeZero;
    _rawFrameSize = CGSizeZero;
    _renderSize = CGSizeZero;
    _rotation = -1;
    if (videoTrack) {
      [videoTrack addRenderer:self];
    }
  }
}

- (id<RTCI420Buffer>)correctRotation:(const id<RTCI420Buffer>)src
                        withRotation:(RTCVideoRotation)rotation {
  int rotated_width = src.width;
  int rotated_height = src.height;

  if (rotation == RTCVideoRotation_90 || rotation == RTCVideoRotation_270) {
    int temp = rotated_width;
    rotated_width = rotated_height;
    rotated_height = temp;
  }

  id<RTCI420Buffer> buffer = [[RTCI420Buffer alloc] initWithWidth:rotated_width
                                                           height:rotated_height];

  [RTCYUVHelper I420Rotate:src.dataY
                srcStrideY:src.strideY
                      srcU:src.dataU
                srcStrideU:src.strideU
                      srcV:src.dataV
                srcStrideV:src.strideV
                      dstY:(uint8_t*)buffer.dataY
                dstStrideY:buffer.strideY
                      dstU:(uint8_t*)buffer.dataU
                dstStrideU:buffer.strideU
                      dstV:(uint8_t*)buffer.dataV
                dstStrideV:buffer.strideV
                     width:src.width
                    height:src.height
                      mode:rotation];

  return buffer;
}

- (void)copyI420ToCVPixelBuffer:(CVPixelBufferRef)outputPixelBuffer
                      withFrame:(RTCVideoFrame*)frame {
  id<RTCI420Buffer> i420Buffer = [self correctRotation:[frame.buffer toI420]
                                          withRotation:frame.rotation];
  CVPixelBufferLockBaseAddress(outputPixelBuffer, 0);

  const OSType pixelFormat = CVPixelBufferGetPixelFormatType(outputPixelBuffer);
  if (pixelFormat == kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange ||
      pixelFormat == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange) {
    // NV12
    uint8_t* dstY = CVPixelBufferGetBaseAddressOfPlane(outputPixelBuffer, 0);
    const size_t dstYStride = CVPixelBufferGetBytesPerRowOfPlane(outputPixelBuffer, 0);
    uint8_t* dstUV = CVPixelBufferGetBaseAddressOfPlane(outputPixelBuffer, 1);
    const size_t dstUVStride = CVPixelBufferGetBytesPerRowOfPlane(outputPixelBuffer, 1);

    [RTCYUVHelper I420ToNV12:i420Buffer.dataY
                  srcStrideY:i420Buffer.strideY
                        srcU:i420Buffer.dataU
                  srcStrideU:i420Buffer.strideU
                        srcV:i420Buffer.dataV
                  srcStrideV:i420Buffer.strideV
                        dstY:dstY
                  dstStrideY:(int)dstYStride
                       dstUV:dstUV
                 dstStrideUV:(int)dstUVStride
                       width:i420Buffer.width
                      height:i420Buffer.height];

  } else {
    uint8_t* dst = CVPixelBufferGetBaseAddress(outputPixelBuffer);
    const size_t bytesPerRow = CVPixelBufferGetBytesPerRow(outputPixelBuffer);

    if (pixelFormat == kCVPixelFormatType_32BGRA) {
      // Corresponds to libyuv::FOURCC_ARGB

      [RTCYUVHelper I420ToARGB:i420Buffer.dataY
                    srcStrideY:i420Buffer.strideY
                          srcU:i420Buffer.dataU
                    srcStrideU:i420Buffer.strideU
                          srcV:i420Buffer.dataV
                    srcStrideV:i420Buffer.strideV
                       dstARGB:dst
                 dstStrideARGB:(int)bytesPerRow
                         width:i420Buffer.width
                        height:i420Buffer.height];

    } else if (pixelFormat == kCVPixelFormatType_32ARGB) {
      // Corresponds to libyuv::FOURCC_BGRA
      [RTCYUVHelper I420ToBGRA:i420Buffer.dataY
                    srcStrideY:i420Buffer.strideY
                          srcU:i420Buffer.dataU
                    srcStrideU:i420Buffer.strideU
                          srcV:i420Buffer.dataV
                    srcStrideV:i420Buffer.strideV
                       dstBGRA:dst
                 dstStrideBGRA:(int)bytesPerRow
                         width:i420Buffer.width
                        height:i420Buffer.height];
    }
  }

  CVPixelBufferUnlockBaseAddress(outputPixelBuffer, 0);
}

#pragma mark - RTCVideoRenderer methods
- (void)renderFrame:(RTCVideoFrame*)frame {

  os_unfair_lock_lock(&_lock);
  if(_videoTrack == nil) {
    os_unfair_lock_unlock(&_lock);
    return;
  }
  if(!_frameAvailable && _pixelBufferRef) {
    if (_dewarpConfig != nil && _rawPixelBufferRef != nil) {
      [self copyI420ToCVPixelBuffer:_rawPixelBufferRef withFrame:frame];
      [self.dewarpProcessor renderConfig:_dewarpConfig
                        sourcePixelBuffer:_rawPixelBufferRef
                          destPixelBuffer:_pixelBufferRef];
    } else {
      [self copyI420ToCVPixelBuffer:_pixelBufferRef withFrame:frame];
    }
    if(_textureId != -1) {
      [_registry textureFrameAvailable:_textureId];
    }
    _frameAvailable = true;
  }
  // _frameSize is what _pixelBufferRef actually holds -- the raw decoded
  // size normally, or the dewarped composite canvas size when dewarp is
  // configured (see -setSize:). Report that, not the raw frame.width /
  // frame.height, so RTCVideoView's aspect-ratio-driven layout on the
  // Dart side matches what's actually on screen.
  CGSize reportedSize = _frameSize;
  os_unfair_lock_unlock(&_lock);

  __weak FlutterRTCVideoRenderer* weakSelf = self;
  if (_renderSize.width != reportedSize.width || _renderSize.height != reportedSize.height) {
    dispatch_async(dispatch_get_main_queue(), ^{
      FlutterRTCVideoRenderer* strongSelf = weakSelf;
      if (strongSelf.eventSink) {
        strongSelf.eventSink(@{
          @"event" : @"didTextureChangeVideoSize",
          @"id" : @(strongSelf.textureId),
          @"width" : @(reportedSize.width),
          @"height" : @(reportedSize.height),
        });
      }
    });
    _renderSize = reportedSize;
  }

  if (frame.rotation != _rotation) {
    dispatch_async(dispatch_get_main_queue(), ^{
      FlutterRTCVideoRenderer* strongSelf = weakSelf;
      if (strongSelf.eventSink) {
        strongSelf.eventSink(@{
          @"event" : @"didTextureChangeRotation",
          @"id" : @(strongSelf.textureId),
          @"rotation" : @(frame.rotation),
        });
      }
    });

    _rotation = frame.rotation;
  }

  // Notify the Flutter new pixelBufferRef to be ready.
  dispatch_async(dispatch_get_main_queue(), ^{
    FlutterRTCVideoRenderer* strongSelf = weakSelf;
    if (!strongSelf->_isFirstFrameRendered) {
      if (strongSelf.eventSink) {
        strongSelf.eventSink(@{@"event" : @"didFirstFrameRendered"});
        strongSelf->_isFirstFrameRendered = true;
      }
    }
  });
}

/**
 * Sets the size of the video frame to render.
 *
 * @param size The size of the video frame to render.
 */
- (void)setSize:(CGSize)size {
  os_unfair_lock_lock(&_lock);
  BOOL rawSizeChanged =
      (size.width != _rawFrameSize.width || size.height != _rawFrameSize.height);
  _rawFrameSize = size;

  CGSize targetSize = _dewarpConfig != nil
      ? [RTCDewarpProcessor compositeSizeForConfig:_dewarpConfig decodedSize:size]
      : size;
  if (targetSize.width != _frameSize.width || targetSize.height != _frameSize.height) {
    if (_pixelBufferRef) {
      CVBufferRelease(_pixelBufferRef);
    }
    NSDictionary* pixelAttributes = @{(id)kCVPixelBufferIOSurfacePropertiesKey : @{}};
    CVPixelBufferCreate(kCFAllocatorDefault, targetSize.width, targetSize.height,
                        kCVPixelFormatType_32BGRA, (__bridge CFDictionaryRef)(pixelAttributes),
                        &_pixelBufferRef);
    _frameAvailable = false;
    _frameSize = targetSize;
  }

  if (_dewarpConfig != nil) {
    if (rawSizeChanged || _rawPixelBufferRef == nil) {
      if (_rawPixelBufferRef) {
        CVBufferRelease(_rawPixelBufferRef);
      }
      NSDictionary* rawPixelAttributes = @{(id)kCVPixelBufferIOSurfacePropertiesKey : @{}};
      CVPixelBufferCreate(kCFAllocatorDefault, size.width, size.height, kCVPixelFormatType_32BGRA,
                          (__bridge CFDictionaryRef)(rawPixelAttributes), &_rawPixelBufferRef);
    }
  } else if (_rawPixelBufferRef) {
    CVBufferRelease(_rawPixelBufferRef);
    _rawPixelBufferRef = nil;
  }
  os_unfair_lock_unlock(&_lock);
}

- (RTCDewarpProcessor*)dewarpProcessor {
  if (_dewarpProcessor == nil) {
    _dewarpProcessor = [[RTCDewarpProcessor alloc] init];
  }
  return _dewarpProcessor;
}

- (void)setDewarpConfig:(RTCDewarpConfig* _Nullable)config {
  os_unfair_lock_lock(&_lock);
  _dewarpConfig = config;
  CGSize rawSize = _rawFrameSize;
  os_unfair_lock_unlock(&_lock);
  if (!CGSizeEqualToSize(rawSize, CGSizeZero)) {
    [self setSize:rawSize];
  }
}

- (void)updatePtzTilePan:(NSInteger)tileIndex panDeg:(float)panDeg {
  os_unfair_lock_lock(&_lock);
  if (_dewarpConfig != nil && tileIndex >= 0 && tileIndex < (NSInteger)_dewarpConfig.ptzTiles.count) {
    _dewarpConfig.ptzTiles[tileIndex].panDeg = panDeg;
  }
  os_unfair_lock_unlock(&_lock);
}

- (void)updateBaseTileOffset:(float)panDeg tiltDeg:(float)tiltDeg {
  os_unfair_lock_lock(&_lock);
  if (_dewarpConfig != nil) {
    _dewarpConfig.basePanDeg = panDeg;
    _dewarpConfig.baseTiltDeg = tiltDeg;
  }
  os_unfair_lock_unlock(&_lock);
}

#pragma mark - FlutterStreamHandler methods

- (FlutterError* _Nullable)onCancelWithArguments:(id _Nullable)arguments {
  _eventSink = nil;
  return nil;
}

- (FlutterError* _Nullable)onListenWithArguments:(id _Nullable)arguments
                                       eventSink:(nonnull FlutterEventSink)sink {
  _eventSink = sink;
  return nil;
}
@end

@implementation FlutterWebRTCPlugin (FlutterVideoRendererManager)

- (FlutterRTCVideoRenderer*)createWithTextureRegistry:(id<FlutterTextureRegistry>)registry
                                            messenger:(NSObject<FlutterBinaryMessenger>*)messenger {
  return [[FlutterRTCVideoRenderer alloc] initWithTextureRegistry:registry messenger:messenger];
}

- (void)rendererSetSrcObject:(FlutterRTCVideoRenderer*)renderer stream:(RTCVideoTrack*)videoTrack {
  renderer.videoTrack = videoTrack;
}
@end
