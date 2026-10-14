#import <Flutter/Flutter.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * Objective-C counterpart to Android's MediaSdkFisheyePlayer.java +
 * mediasdk_jni.cpp -- see fisheye/README.md for the reference C call
 * sequence this mirrors exactly (Init -> CreatPortRes ->
 * SetDecodeVideoDataCB -> SetVideoCallBackMode(PROCESSED_DATA) ->
 * StartStream(RTSP realplay, pWnd=NULL) -> SetPtzAndFixMode -> ... ->
 * StopStream -> DestroyPortRes), calling straight into the vendored
 * libMediaSDK.framework C API (fisheye/ios/vendor/Frameworks).
 *
 * Rendering works differently from Android's SurfaceTextureRenderer
 * reuse -- Flutter's iOS texture registry is pull-based (the engine
 * calls -copyPixelBuffer whenever it wants a frame, unlike Android's
 * push-based SurfaceProducer), so this class implements <FlutterTexture>
 * directly. Each corrected YUV420 frame is converted to a BGRA
 * CVPixelBuffer (matching the format FlutterRTCVideoRenderer's own
 * -copyPixelBuffer produces) since Flutter's Metal-backed texture cache
 * expects an IOSurface-backed, GPU-uploadable pixel format -- true
 * triplanar 4:2:0 (three separate Y/Cb/Cr planes, no interleaving) isn't
 * one of those, unlike NV12's biplanar layout. No fisheye-specific GL/
 * Metal drawer is needed -- MediaSDK already did the dewarping, this
 * only has to display an already-correct frame.
 */
@interface MediaSdkFisheyePlayer : NSObject <FlutterTexture>

/**
 * Frame arrival is reported through this sink as
 * {@"event": @"onFisheyeMediaSdkFrame", @"port":..., @"width":...,
 * @"height":..., @"dataType":..., @"frameCount":...} -- the same shape
 * as the Android side's event so Dart doesn't need per-platform parsing.
 * Set this to FlutterWebRTCPlugin's own eventSink so frames flow through
 * the same "FlutterWebRTC.Event" channel as everything else.
 */
@property(nonatomic, copy, nullable) FlutterEventSink eventSink;

/**
 * Returns YES if every setup step succeeded; check the Xcode/device
 * console (prefix "[MediaSdkFisheyePlayer]") for the failing step and
 * MediaSDK error code otherwise. MediaSDK's own StartStream blocks until
 * the RTSP session connects or fails, so call this off the main thread.
 */
- (BOOL)startWithRtspUrl:(NSString *)rtspUrl ptzMode:(NSUInteger)ptzMode fixMode:(NSUInteger)fixMode;

/**
 * Changes the fisheye correction mode on the already-running stream
 * without reconnecting -- see the .m file's doc for why this exists
 * separately from -startWithRtspUrl:ptzMode:fixMode:. No-op (returns NO)
 * if the stream hasn't been started yet.
 */
- (BOOL)setPtzMode:(NSUInteger)ptzMode fixMode:(NSUInteger)fixMode;

- (void)stop;

/**
 * Registers this instance as a Flutter texture and returns its
 * textureId (pass straight to a `Texture(textureId: ...)` widget on the
 * Dart side). Frames simply have nowhere to go until this is called.
 */
- (int64_t)createTextureWithRegistry:(id<FlutterTextureRegistry>)registry;

- (void)disposeTexture;

@end

NS_ASSUME_NONNULL_END
