#import "FlutterWebRTCPlugin.h"
#import "RTCDewarpConfig.h"

#import <WebRTC/RTCMediaStream.h>
#import <WebRTC/RTCVideoFrame.h>
#import <WebRTC/RTCVideoRenderer.h>
#import <WebRTC/RTCVideoTrack.h>

@interface FlutterRTCVideoRenderer
    : NSObject <FlutterTexture, RTCVideoRenderer, FlutterStreamHandler>

/**
 * The {@link RTCVideoTrack}, if any, which this instance renders.
 */
@property(nonatomic, strong) RTCVideoTrack* videoTrack;
@property(nonatomic) int64_t textureId;
@property(nonatomic, weak) id<FlutterTextureRegistry> registry;
@property(nonatomic, strong) FlutterEventSink eventSink;

- (instancetype)initWithTextureRegistry:(id<FlutterTextureRegistry>)registry
                              messenger:(NSObject<FlutterBinaryMessenger>*)messenger;

- (void)dispose;

/**
 * Configures (or, passing nil, clears) real-time fisheye lens dewarping
 * for this renderer. Takes effect on the next frame; if a frame of a
 * known size has already been rendered, buffer sizes are recomputed and
 * reallocated immediately rather than waiting for the next
 * RTCVideoRenderer -setSize: callback from WebRTC.
 *
 * This never touches videoTrack or anything at the WebRTC/peer-connection
 * layer -- renderFrame: keeps being invoked by the same, still-attached
 * track throughout. Only the pixels written into the Flutter texture
 * change (raw frame vs. dewarped composite); the stream itself is never
 * interrupted. Safe to call whenever a stream is already playing on this
 * renderer.
 */
- (void)setDewarpConfig:(RTCDewarpConfig* _Nullable)config;

@end

@interface FlutterWebRTCPlugin (FlutterVideoRendererManager)

- (FlutterRTCVideoRenderer*)createWithTextureRegistry:(id<FlutterTextureRegistry>)registry
                                            messenger:(NSObject<FlutterBinaryMessenger>*)messenger;

- (void)rendererSetSrcObject:(FlutterRTCVideoRenderer*)renderer stream:(RTCVideoTrack*)videoTrack;

@end
