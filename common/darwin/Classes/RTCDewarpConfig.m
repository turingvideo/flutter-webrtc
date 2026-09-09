#import "RTCDewarpConfig.h"

NSString* const RTCDewarpConfigErrorDomain = @"FlutterWebRTC.RTCDewarpConfig";

@implementation RTCDewarpPtzTile

- (instancetype)initWithPanDeg:(float)panDeg tiltDeg:(float)tiltDeg fovDeg:(float)fovDeg {
  self = [super init];
  if (self) {
    _panDeg = panDeg;
    _tiltDeg = tiltDeg;
    _fovDeg = fovDeg;
  }
  return self;
}

@end

@implementation RTCDewarpConfig

- (instancetype)initWithMountType:(RTCDewarpMountType)mountType
                       displayMode:(RTCDewarpDisplayMode)displayMode
                       centerXNorm:(float)centerXNorm
                       centerYNorm:(float)centerYNorm
                        radiusNorm:(float)radiusNorm
                       rotationDeg:(float)rotationDeg
                          ptzTiles:(NSArray<RTCDewarpPtzTile*>*)ptzTiles {
  self = [super init];
  if (self) {
    _mountType = mountType;
    _displayMode = displayMode;
    _centerXNorm = centerXNorm;
    _centerYNorm = centerYNorm;
    _radiusNorm = radiusNorm;
    _rotationDeg = rotationDeg;
    _ptzTiles = [ptzTiles copy];

    // Mirrors DewarpConfig.DisplayMode's per-mode table on Android
    // (android/src/main/java/com/cloudwebrtc/webrtc/DewarpConfig.java).
    switch (displayMode) {
      case RTCDewarpDisplayModeFisheye:
        _panoramaArcSpanDeg = 0;
        _panoramaSplitCount = 0;
        _ptzTileCount = 0;
        break;
      case RTCDewarpDisplayModePano180:
        _panoramaArcSpanDeg = 360;
        _panoramaSplitCount = 2;
        _ptzTileCount = 0;
        break;
      case RTCDewarpDisplayModePanoramic:
        _panoramaArcSpanDeg = 180;
        _panoramaSplitCount = 1;
        _ptzTileCount = 0;
        break;
      case RTCDewarpDisplayModeThreeSixtyPlus1Ptz:
        _panoramaArcSpanDeg = 360;
        _panoramaSplitCount = 1;
        _ptzTileCount = 1;
        break;
      case RTCDewarpDisplayModeThreeSixtyPlus6Ptz:
        _panoramaArcSpanDeg = 360;
        _panoramaSplitCount = 1;
        _ptzTileCount = 6;
        break;
      case RTCDewarpDisplayModeFisheyePlus3Ptz:
        _panoramaArcSpanDeg = 0;
        _panoramaSplitCount = 0;
        _ptzTileCount = 3;
        break;
      case RTCDewarpDisplayModeFisheyePlus4Ptz:
        _panoramaArcSpanDeg = 0;
        _panoramaSplitCount = 0;
        _ptzTileCount = 4;
        break;
      case RTCDewarpDisplayModeFisheyePlus8Ptz:
        _panoramaArcSpanDeg = 0;
        _panoramaSplitCount = 0;
        _ptzTileCount = 8;
        break;
      case RTCDewarpDisplayModePanoPlus3Ptz:
        _panoramaArcSpanDeg = 180;
        _panoramaSplitCount = 1;
        _ptzTileCount = 3;
        break;
      case RTCDewarpDisplayModePanoPlus4Ptz:
        _panoramaArcSpanDeg = 180;
        _panoramaSplitCount = 1;
        _ptzTileCount = 4;
        break;
      case RTCDewarpDisplayModePanoPlus8Ptz:
        _panoramaArcSpanDeg = 180;
        _panoramaSplitCount = 1;
        _ptzTileCount = 8;
        break;
    }
    _usesPanoramaBase = _panoramaArcSpanDeg > 0;
    _baseTileHeightFraction =
        displayMode == RTCDewarpDisplayModeThreeSixtyPlus1Ptz ? (2.0f / 3.0f) : 0.5f;
    _usesPanoramaPtzTiles = displayMode == RTCDewarpDisplayModeThreeSixtyPlus1Ptz;
    _usesDirectCropBase = displayMode == RTCDewarpDisplayModeThreeSixtyPlus1Ptz;
    _basePanDeg = 0.0f;
    _baseTiltDeg = 0.0f;
  }
  return self;
}

static BOOL RTCDewarpMountTypeFromWireName(NSString* wireName, RTCDewarpMountType* outValue) {
  if ([wireName isEqualToString:@"ceiling"]) {
    *outValue = RTCDewarpMountTypeCeiling;
  } else if ([wireName isEqualToString:@"desktop"]) {
    *outValue = RTCDewarpMountTypeDesktop;
  } else if ([wireName isEqualToString:@"wall"]) {
    *outValue = RTCDewarpMountTypeWall;
  } else {
    return NO;
  }
  return YES;
}

static BOOL RTCDewarpDisplayModeFromWireName(NSString* wireName, RTCDewarpDisplayMode* outValue) {
  static NSDictionary<NSString*, NSNumber*>* map;
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    map = @{
      @"fisheye" : @(RTCDewarpDisplayModeFisheye),
      @"pano180" : @(RTCDewarpDisplayModePano180),
      @"panoramic" : @(RTCDewarpDisplayModePanoramic),
      @"threeSixtyPlus1Ptz" : @(RTCDewarpDisplayModeThreeSixtyPlus1Ptz),
      @"threeSixtyPlus6Ptz" : @(RTCDewarpDisplayModeThreeSixtyPlus6Ptz),
      @"fisheyePlus3Ptz" : @(RTCDewarpDisplayModeFisheyePlus3Ptz),
      @"fisheyePlus4Ptz" : @(RTCDewarpDisplayModeFisheyePlus4Ptz),
      @"fisheyePlus8Ptz" : @(RTCDewarpDisplayModeFisheyePlus8Ptz),
      @"panoPlus3Ptz" : @(RTCDewarpDisplayModePanoPlus3Ptz),
      @"panoPlus4Ptz" : @(RTCDewarpDisplayModePanoPlus4Ptz),
      @"panoPlus8Ptz" : @(RTCDewarpDisplayModePanoPlus8Ptz),
    };
  });
  NSNumber* value = map[wireName];
  if (value == nil) return NO;
  *outValue = (RTCDewarpDisplayMode)value.integerValue;
  return YES;
}

static NSError* RTCDewarpConfigError(NSString* message) {
  return [NSError errorWithDomain:RTCDewarpConfigErrorDomain
                              code:0
                          userInfo:@{NSLocalizedDescriptionKey : message}];
}

static float RTCDewarpFloatValue(NSDictionary* dict, NSString* key, float defaultValue) {
  NSNumber* value = dict[key];
  return value == nil ? defaultValue : value.floatValue;
}

+ (nullable instancetype)configFromDictionary:(NSDictionary*)dict error:(NSError**)error {
  RTCDewarpMountType mountType;
  NSString* mountTypeWireName = dict[@"mountType"];
  if (![mountTypeWireName isKindOfClass:[NSString class]] ||
      !RTCDewarpMountTypeFromWireName(mountTypeWireName, &mountType)) {
    if (error) {
      *error = RTCDewarpConfigError(
          [NSString stringWithFormat:@"Unknown FisheyeMountType: %@", mountTypeWireName]);
    }
    return nil;
  }

  RTCDewarpDisplayMode displayMode;
  NSString* displayModeWireName = dict[@"displayMode"];
  if (![displayModeWireName isKindOfClass:[NSString class]] ||
      !RTCDewarpDisplayModeFromWireName(displayModeWireName, &displayMode)) {
    if (error) {
      *error = RTCDewarpConfigError(
          [NSString stringWithFormat:@"Unknown FisheyeDisplayMode: %@", displayModeWireName]);
    }
    return nil;
  }

  NSDictionary* calibration = dict[@"calibration"];
  if (![calibration isKindOfClass:[NSDictionary class]]) calibration = @{};
  float centerXNorm = RTCDewarpFloatValue(calibration, @"centerXNorm", 0.5f);
  float centerYNorm = RTCDewarpFloatValue(calibration, @"centerYNorm", 0.5f);
  float radiusNorm = RTCDewarpFloatValue(calibration, @"radiusNorm", 0.5f);
  float rotationDeg = RTCDewarpFloatValue(calibration, @"rotationDeg", 0.0f);

  NSMutableArray<RTCDewarpPtzTile*>* ptzTiles = [NSMutableArray array];
  NSArray* rawTiles = dict[@"ptzTiles"];
  if ([rawTiles isKindOfClass:[NSArray class]]) {
    for (NSDictionary* rawTile in rawTiles) {
      if (![rawTile isKindOfClass:[NSDictionary class]]) continue;
      [ptzTiles addObject:[[RTCDewarpPtzTile alloc]
                               initWithPanDeg:RTCDewarpFloatValue(rawTile, @"panDeg", 0.0f)
                                      tiltDeg:RTCDewarpFloatValue(rawTile, @"tiltDeg", 0.0f)
                                       fovDeg:RTCDewarpFloatValue(rawTile, @"fovDeg", 90.0f)]];
    }
  }

  RTCDewarpConfig* config = [[RTCDewarpConfig alloc] initWithMountType:mountType
                                                             displayMode:displayMode
                                                             centerXNorm:centerXNorm
                                                             centerYNorm:centerYNorm
                                                              radiusNorm:radiusNorm
                                                             rotationDeg:rotationDeg
                                                                ptzTiles:ptzTiles];
  if ((NSInteger)ptzTiles.count != config.ptzTileCount) {
    if (error) {
      *error = RTCDewarpConfigError([NSString
          stringWithFormat:@"%@ requires %ld ptzTiles, got %lu", displayModeWireName,
                            (long)config.ptzTileCount, (unsigned long)ptzTiles.count]);
    }
    return nil;
  }

  return config;
}

@end
