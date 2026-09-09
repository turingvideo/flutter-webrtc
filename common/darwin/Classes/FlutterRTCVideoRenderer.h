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

/**
 * Pans a single PTZ tile of the current dewarp config in place, for
 * drag-to-scroll interactions -- unlike -setDewarpConfig:, this never
 * reallocates any buffers, since panning doesn't change the composite
 * canvas size, only what's sampled into it. No-op if there is no active
 * dewarp config or `tileIndex` is out of range (e.g. a stale call racing a
 * mode switch that just cleared/replaced the config). Matches
 * FlutterRTCVideoRenderer#updatePtzTilePan on Android exactly.
 */
- (void)updatePtzTilePan:(NSInteger)tileIndex panDeg:(float)panDeg;

/**
 * Same idea as -updatePtzTilePan:panDeg:, but for the base tile's own
 * independent pan+tilt offset (see RTCDewarpConfig.basePanDeg/
 * baseTiltDeg) -- only meaningful when the current display mode's base
 * tile is a direct rectangular crop (see
 * RTCDewarpConfig.usesDirectCropBase). Takes both axes in one call since a
 * drag gesture naturally produces both at once. Matches
 * FlutterRTCVideoRenderer#updateBaseTileOffset on Android exactly.
 */
- (void)updateBaseTileOffset:(float)panDeg tiltDeg:(float)tiltDeg;

@end

@interface FlutterWebRTCPlugin (FlutterVideoRendererManager)

- (FlutterRTCVideoRenderer*)createWithTextureRegistry:(id<FlutterTextureRegistry>)registry
                                            messenger:(NSObject<FlutterBinaryMessenger>*)messenger;

- (void)rendererSetSrcObject:(FlutterRTCVideoRenderer*)renderer stream:(RTCVideoTrack*)videoTrack;

@end
