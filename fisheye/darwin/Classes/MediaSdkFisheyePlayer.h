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
 * Same scope as the Android side: proves the pipeline runs and delivers
 * corrected YUV frames (reported via eventSink below) -- no rendering
 * path yet. See MediaSdkFisheyePlayer.java's class doc for the bigger
 * picture and what's still missing before this can show video on
 * screen.
 */
@interface MediaSdkFisheyePlayer : NSObject

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

- (void)stop;

@end

NS_ASSUME_NONNULL_END
