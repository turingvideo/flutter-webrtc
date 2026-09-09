#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * Physical installation of the fisheye lens. Determines which
 * RTCDewarpDisplayMode values are valid and how the projection math
 * orients the source image.
 *
 * Mirrors Dart's FisheyeMountType (lib/src/native/rtc_video_dewarp.dart).
 */
typedef NS_ENUM(NSInteger, RTCDewarpMountType) {
  RTCDewarpMountTypeCeiling,
  RTCDewarpMountTypeDesktop,
  RTCDewarpMountTypeWall,
};

/**
 * How a dewarped fisheye stream should be laid out on screen. Not every
 * mode is valid for every RTCDewarpMountType -- see
 * RTCDewarpConfigDictionaryToConfig / the Dart-side
 * kValidFisheyeDisplayModes for the validity matrix.
 *
 * Mirrors Dart's FisheyeDisplayMode.
 */
typedef NS_ENUM(NSInteger, RTCDewarpDisplayMode) {
  RTCDewarpDisplayModeFisheye,
  RTCDewarpDisplayModePano180,
  RTCDewarpDisplayModePanoramic,
  RTCDewarpDisplayModeThreeSixtyPlus1Ptz,
  RTCDewarpDisplayModeThreeSixtyPlus6Ptz,
  RTCDewarpDisplayModeFisheyePlus3Ptz,
  RTCDewarpDisplayModeFisheyePlus4Ptz,
  RTCDewarpDisplayModeFisheyePlus8Ptz,
  RTCDewarpDisplayModePanoPlus3Ptz,
  RTCDewarpDisplayModePanoPlus4Ptz,
  RTCDewarpDisplayModePanoPlus8Ptz,
};

/** Pan/tilt/zoom of a single virtual PTZ tile carved out of the fisheye image. */
@interface RTCDewarpPtzTile : NSObject

/**
 * Mutable (unlike the rest of RTCDewarpConfig, which is a snapshot from the
 * last "videoRendererSetDewarpConfig" call): FlutterRTCVideoRenderer's
 * updatePtzTilePan:panDeg: mutates this in place so a drag gesture can pan
 * in real time without paying for a full renderer teardown every frame.
 * RTCDewarpProcessor re-reads it fresh every frame, so no extra
 * synchronization is needed for this single-float property -- matches
 * DewarpConfig.PtzTile#panDeg's `volatile` on Android exactly.
 */
@property(nonatomic) float panDeg;
@property(nonatomic, readonly) float tiltDeg;
@property(nonatomic, readonly) float fovDeg;

- (instancetype)initWithPanDeg:(float)panDeg tiltDeg:(float)tiltDeg fovDeg:(float)fovDeg;

@end

/**
 * Fisheye lens dewarp configuration for a single FlutterRTCVideoRenderer.
 *
 * Mirrors the Dart-side FisheyeDewarpConfig payload sent over the
 * "videoRendererSetDewarpConfig" method channel call. String values in the
 * source dictionary match the Dart enums' `.name` exactly.
 */
@interface RTCDewarpConfig : NSObject

@property(nonatomic, readonly) RTCDewarpMountType mountType;
@property(nonatomic, readonly) RTCDewarpDisplayMode displayMode;

@property(nonatomic, readonly) float centerXNorm;
@property(nonatomic, readonly) float centerYNorm;
@property(nonatomic, readonly) float radiusNorm;
@property(nonatomic, readonly) float rotationDeg;

@property(nonatomic, readonly, copy) NSArray<RTCDewarpPtzTile*>* ptzTiles;

/**
 * Independent pan for the base tile, only meaningful when
 * usesPanoramaPtzTiles is true: in that case the base tile is *also* a
 * pannable crop of the panorama (same FOV as ptzTiles[0], per product's
 * "both windows default to the same zoom, but pan independently"
 * requirement), not a single fixed full-arc flatten. Mutable for the same
 * drag-in-real-time reason as RTCDewarpPtzTile.panDeg; defaults to 0
 * (unrotated). Matches DewarpConfig.basePanDeg on Android exactly.
 */
@property(nonatomic) float basePanDeg;

/**
 * Vertical counterpart to basePanDeg, only meaningful when
 * usesDirectCropBase is true (in that mode the base tile pans on both
 * axes, unlike a PTZ tile crop which is pan-only). Matches
 * DewarpConfig.baseTiltDeg on Android exactly.
 */
@property(nonatomic) float baseTiltDeg;

/**
 * A display mode's base tile is either the raw fisheye circle
 * (panoramaArcSpanDeg == 0) or a panorama strip unwrapped from this arc
 * span, optionally split into panoramaSplitCount stacked strips (only
 * meaningful when panoramaArcSpanDeg == 360; see RTCDewarpProcessor).
 */
@property(nonatomic, readonly) float panoramaArcSpanDeg;
@property(nonatomic, readonly) NSInteger panoramaSplitCount;
@property(nonatomic, readonly) BOOL usesPanoramaBase;

/** Number of virtual PTZ tiles composited alongside the base tile. */
@property(nonatomic, readonly) NSInteger ptzTileCount;

/**
 * Fraction of the composite canvas height given to the base tile (the rest
 * goes to the PTZ tile grid). Placeholder pending pixel-accurate UI
 * mockups, same status as RTCDewarpProcessor's constants -- only
 * RTCDewarpDisplayModeThreeSixtyPlus1Ptz deviates from the shared 0.5
 * default so far (product wants the overview strip to dominate: 2/3
 * overview, 1/3 scrollable close-up). Matches
 * DewarpConfig.DisplayMode#baseTileHeightFraction() on Android exactly.
 */
@property(nonatomic, readonly) float baseTileHeightFraction;

/**
 * Whether this mode's PTZ tile(s) are a scrollable crop of the same
 * cylindrical panorama projection as the base tile (pan-only, no
 * independent tilt/perspective) rather than an independent rectilinear
 * virtual-PTZ camera. Only RTCDewarpDisplayModeThreeSixtyPlus1Ptz does this
 * so far. Matches DewarpConfig.DisplayMode#usesPanoramaPtzTiles() on
 * Android exactly.
 */
@property(nonatomic, readonly) BOOL usesPanoramaPtzTiles;

/**
 * Whether this mode's *base* tile is a plain rectangular crop of the raw
 * fisheye circle -- pan+tilt, no theta/phi reprojection at all -- instead
 * of a cylindrical panorama unwrap or a fixed full-arc flatten. Bounded
 * and edge-clamped rather than black-on-out-of-range, per product's "acts
 * like zooming into the original circular image, never shows black"
 * requirement. Only RTCDewarpDisplayModeThreeSixtyPlus1Ptz does this so
 * far; its PTZ tile is unaffected (still governed by
 * usesPanoramaPtzTiles). Matches
 * DewarpConfig.DisplayMode#usesDirectCropBase() on Android exactly.
 */
@property(nonatomic, readonly) BOOL usesDirectCropBase;

/**
 * Parses a config sent from Dart. Returns nil and populates `error` if
 * `dict` is malformed (unknown enum wire name, or ptzTiles.count doesn't
 * match the display mode's required tile count).
 */
+ (nullable instancetype)configFromDictionary:(NSDictionary*)dict error:(NSError**)error;

@end

NS_ASSUME_NONNULL_END
