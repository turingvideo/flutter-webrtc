#import "RTCDewarpProcessor.h"

#import <CoreImage/CoreImage.h>

// Vertical field of view of Primitive B's per-column "pushbroom" camera
// (see kPanoramaWarpKernelSource's doc below). Placeholder pending real
// product tuning, same status as RTCDewarpConfig.baseTileHeightFraction.
// Matches DewarpGlDrawer.STRIP_VERTICAL_FOV_DEG on Android exactly.
static const CGFloat kStripVerticalFovDeg = 100.0;

// Core Image Kernel Language source for the two projection primitives that
// need a per-pixel warp (the raw-fisheye passthrough primitive needs no
// kernel at all -- see -baseTileImageForConfig:sourceImage:tileRect:).
//
// Both kernels implement the shared equidistant fisheye sampling step:
// given a destination ray's (theta, phi), compute the corresponding
// source pixel. `center`/`radius` are calibrated top-down (y=0 at the top
// of the displayed frame, matching a Flutter overlay's natural coordinate
// system) but Core Image's own coordinate space is bottom-up, hence the
// "1.0 - texYTopDown" flip before returning the source coordinate -- see
// the identical comment in DewarpGlDrawer.java's FISHEYE_SAMPLE_FUNCTION.
// Vertically this does NOT map linearly to theta -- see the matching doc
// comment on PRIMITIVE_B_FRAGMENT_SHADER in DewarpGlDrawer.java (the
// Android counterpart) for the full derivation. In short: each column is
// treated as its own zero-width rectilinear ("pushbroom") camera pointed
// at the horizon, so v=1.0 (top of tile) always lands on the horizon
// (theta=THETA_MAX), with theta decreasing toward the lens' own
// zenith/nadir as v decreases to 0.0, and verticals stay straight,
// matching how real fisheye-camera "panorama" dewarp modes behave. ndcV
// only spans [0,1], not [-1,1]: theta can never legitimately exceed
// THETA_MAX, so the other half of a symmetric sweep would just be the
// out-of-bounds vec2(-1,-1) fallback below, wasting half the tile's
// height on unsampled pixels. halfTanStripVFov is a placeholder pending
// real product tuning, same status as kBaseTileHeightFraction above.
static NSString* const kPanoramaWarpKernelSource =
    @"kernel vec2 panoramaWarp(float tileOriginX, float tileOriginY, float tileWidth,\n"
    @"    float tileHeight, float srcWidth, float srcHeight, float centerXNorm,\n"
    @"    float centerYNorm, float radiusNorm, float rotationOffsetRad,\n"
    @"    float verticalFlipSign, float arcPerStripRad, float stripStartRad,\n"
    @"    float halfTanStripVFov)\n"
    @"{\n"
    @"  vec2 d = destCoord();\n"
    @"  float u = (d.x - tileOriginX) / tileWidth;\n"
    @"  float v = (d.y - tileOriginY) / tileHeight;\n"
    @"  float ndcV = 1.0 - v;\n"
    @"  float theta = 1.5707963268 - atan(ndcV * halfTanStripVFov);\n"
    @"  if (theta > 1.5707963268) {\n"
    @"    return vec2(-1.0, -1.0);\n"
    @"  }\n"
    @"  float phi = stripStartRad + u * arcPerStripRad;\n"
    @"  float rNorm = theta / 1.5707963268;\n"
    @"  float texXTopDown = centerXNorm + rNorm * radiusNorm * cos(phi + rotationOffsetRad);\n"
    @"  float texYTopDown = centerYNorm\n"
    @"      + rNorm * radiusNorm * sin(phi + rotationOffsetRad) * verticalFlipSign;\n"
    @"  if (texXTopDown < 0.0 || texXTopDown > 1.0 || texYTopDown < 0.0 || texYTopDown > 1.0) {\n"
    @"    return vec2(-1.0, -1.0);\n"
    @"  }\n"
    @"  return vec2(texXTopDown * srcWidth, (1.0 - texYTopDown) * srcHeight);\n"
    @"}\n";

static NSString* const kPtzWarpKernelSource =
    @"kernel vec2 ptzWarp(float tileOriginX, float tileOriginY, float tileWidth,\n"
    @"    float tileHeight, float srcWidth, float srcHeight, float centerXNorm,\n"
    @"    float centerYNorm, float radiusNorm, float rotationOffsetRad,\n"
    @"    float verticalFlipSign, float panRad, float tiltRad, float halfTanFovH,\n"
    @"    float halfTanFovV)\n"
    @"{\n"
    @"  vec2 d = destCoord();\n"
    @"  float ndcX = ((d.x - tileOriginX) / tileWidth) * 2.0 - 1.0;\n"
    @"  float ndcY = ((d.y - tileOriginY) / tileHeight) * 2.0 - 1.0;\n"
    @"  vec3 dirLocal = normalize(vec3(ndcX * halfTanFovH, ndcY * halfTanFovV, 1.0));\n"
    @"  float ct = cos(tiltRad);\n"
    @"  float st = sin(tiltRad);\n"
    @"  vec3 afterTilt = vec3(dirLocal.x, dirLocal.y * ct - dirLocal.z * st,\n"
    @"      dirLocal.y * st + dirLocal.z * ct);\n"
    @"  float cp = cos(panRad);\n"
    @"  float sp = sin(panRad);\n"
    @"  vec3 dirWorld = vec3(afterTilt.x * cp - afterTilt.y * sp,\n"
    @"      afterTilt.x * sp + afterTilt.y * cp, afterTilt.z);\n"
    @"  float theta = acos(clamp(dirWorld.z, -1.0, 1.0));\n"
    @"  if (theta > 1.5707963268) {\n"
    @"    return vec2(-1.0, -1.0);\n"
    @"  }\n"
    @"  float phi = atan(dirWorld.y, dirWorld.x);\n"
    @"  float rNorm = theta / 1.5707963268;\n"
    @"  float texXTopDown = centerXNorm + rNorm * radiusNorm * cos(phi + rotationOffsetRad);\n"
    @"  float texYTopDown = centerYNorm\n"
    @"      + rNorm * radiusNorm * sin(phi + rotationOffsetRad) * verticalFlipSign;\n"
    @"  if (texXTopDown < 0.0 || texXTopDown > 1.0 || texYTopDown < 0.0 || texYTopDown > 1.0) {\n"
    @"    return vec2(-1.0, -1.0);\n"
    @"  }\n"
    @"  return vec2(texXTopDown * srcWidth, (1.0 - texYTopDown) * srcHeight);\n"
    @"}\n";

@implementation RTCDewarpProcessor {
  CIContext* _ciContext;
  CIWarpKernel* _panoramaKernel;
  CIWarpKernel* _ptzKernel;
}

- (instancetype)init {
  self = [super init];
  if (self) {
    _ciContext = [CIContext context];
  }
  return self;
}

- (CIWarpKernel*)panoramaKernel {
  if (_panoramaKernel == nil) {
    // -kernelWithString: is deprecated in favor of -kernelsWithMetalString:,
    // which requires iOS 15/macOS 12 -- higher than this plugin's iOS
    // 13/macOS 10.15 deployment target. The deprecated API is still fully
    // functional; see the class doc.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    _panoramaKernel = (CIWarpKernel*)[CIWarpKernel kernelWithString:kPanoramaWarpKernelSource];
#pragma clang diagnostic pop
  }
  return _panoramaKernel;
}

- (CIWarpKernel*)ptzKernel {
  if (_ptzKernel == nil) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    _ptzKernel = (CIWarpKernel*)[CIWarpKernel kernelWithString:kPtzWarpKernelSource];
#pragma clang diagnostic pop
  }
  return _ptzKernel;
}

- (void)renderConfig:(RTCDewarpConfig*)config
    sourcePixelBuffer:(CVPixelBufferRef)sourcePixelBuffer
      destPixelBuffer:(CVPixelBufferRef)destPixelBuffer {
  CIImage* sourceImage = [CIImage imageWithCVPixelBuffer:sourcePixelBuffer];
  CGFloat srcWidth = CVPixelBufferGetWidth(sourcePixelBuffer);
  CGFloat srcHeight = CVPixelBufferGetHeight(sourcePixelBuffer);
  CGFloat destWidth = CVPixelBufferGetWidth(destPixelBuffer);
  CGFloat destHeight = CVPixelBufferGetHeight(destPixelBuffer);

  NSInteger ptzTileCount = config.ptzTileCount;
  NSInteger totalTiles = 1 + ptzTileCount;
  for (NSInteger tileIndex = 0; tileIndex < totalTiles; tileIndex++) {
    CGRect tileRect = [self tileRectForIndex:tileIndex
                                 ptzTileCount:ptzTileCount
                    baseTileHeightFraction:config.baseTileHeightFraction
                                   destWidth:destWidth
                                  destHeight:destHeight];
    if (tileIndex == 0) {
      [self renderBaseTileForConfig:config
                          sourceImage:sourceImage
                             srcWidth:srcWidth
                            srcHeight:srcHeight
                             tileRect:tileRect
                      destPixelBuffer:destPixelBuffer];
      continue;
    }
    CIImage* tileImage = [self ptzTileImageForConfig:config
                                                  tile:config.ptzTiles[tileIndex - 1]
                                           sourceImage:sourceImage
                                              srcWidth:srcWidth
                                             srcHeight:srcHeight
                                              tileRect:tileRect];
    if (tileImage == nil) continue;
    [_ciContext render:tileImage toCVPixelBuffer:destPixelBuffer bounds:tileRect colorSpace:nil];
  }
}

- (void)renderBaseTileForConfig:(RTCDewarpConfig*)config
                     sourceImage:(CIImage*)sourceImage
                        srcWidth:(CGFloat)srcWidth
                       srcHeight:(CGFloat)srcHeight
                        tileRect:(CGRect)tileRect
                 destPixelBuffer:(CVPixelBufferRef)destPixelBuffer {
  if (config.usesPanoramaPtzTiles) {
    // Both windows are independent pannable crops of the same panorama, at
    // the same default FOV -- this one is just bigger. See
    // RTCDewarpConfig.basePanDeg's doc.
    float fovDeg = config.ptzTiles.count > 0 ? config.ptzTiles[0].fovDeg : 90.0f;
    CIImage* tileImage = [self panoramaCropImageForConfig:config
                                                    panDeg:config.basePanDeg
                                                    fovDeg:fovDeg
                                               sourceImage:sourceImage
                                                  srcWidth:srcWidth
                                                 srcHeight:srcHeight
                                                  tileRect:tileRect];
    if (tileImage != nil) {
      [_ciContext render:tileImage toCVPixelBuffer:destPixelBuffer bounds:tileRect colorSpace:nil];
    }
    return;
  }
  if (!config.usesPanoramaBase) {
    // Primitive A: raw fisheye passthrough. No warp kernel needed -- just
    // scale+translate the source image to fill the tile rect.
    CGAffineTransform transform =
        CGAffineTransformMakeTranslation(tileRect.origin.x, tileRect.origin.y);
    transform = CGAffineTransformScale(transform, tileRect.size.width / srcWidth,
                                        tileRect.size.height / srcHeight);
    CIImage* tileImage = [sourceImage imageByApplyingTransform:transform];
    [_ciContext render:tileImage toCVPixelBuffer:destPixelBuffer bounds:tileRect colorSpace:nil];
    return;
  }

  CIWarpKernel* kernel = self.panoramaKernel;
  if (kernel == nil) return;
  CGFloat arcPerStripRad =
      (CGFloat)(config.panoramaArcSpanDeg * M_PI / 180.0) / MAX(1, config.panoramaSplitCount);
  NSInteger splitCount = config.panoramaSplitCount;
  // splitCount == 1: a single strip fills the whole base tile rect.
  // splitCount == 2 (the "180°Pano"/pano180 mode): the two 180° strips
  // that together cover the full 360° circle are stacked as the top and
  // bottom halves of the base tile, each rendered separately into its own
  // half-height sub-rect.
  CGFloat stripHeight = MAX(1, tileRect.size.height / splitCount);
  for (NSInteger strip = 0; strip < splitCount; strip++) {
    // Core Image is bottom-up: strip 0 (the first half of the arc) goes in
    // the *top* half of the tile, matching DewarpGlDrawer's convention.
    CGRect stripRect = CGRectMake(tileRect.origin.x,
                                    tileRect.origin.y + tileRect.size.height - (strip + 1) * stripHeight,
                                    tileRect.size.width, stripHeight);
    CIImage* stripImage =
        [kernel applyWithExtent:stripRect
                      roiCallback:^CGRect(int index, CGRect destRect) {
                        return sourceImage.extent;
                      }
                       inputImage:sourceImage
                        arguments:@[
                          @(stripRect.origin.x), @(stripRect.origin.y), @(stripRect.size.width),
                          @(stripRect.size.height), @(srcWidth), @(srcHeight),
                          @(config.centerXNorm), @(config.centerYNorm), @(config.radiusNorm),
                          @(config.rotationDeg * M_PI / 180.0),
                          @(config.mountType == RTCDewarpMountTypeDesktop ? -1.0 : 1.0),
                          @(arcPerStripRad), @(strip * arcPerStripRad),
                          @(tan(kStripVerticalFovDeg * M_PI / 180.0 / 2.0))
                        ]];
    if (stripImage == nil) continue;
    [_ciContext render:stripImage toCVPixelBuffer:destPixelBuffer bounds:stripRect colorSpace:nil];
  }
}

- (nullable CIImage*)ptzTileImageForConfig:(RTCDewarpConfig*)config
                                       tile:(RTCDewarpPtzTile*)tile
                                sourceImage:(CIImage*)sourceImage
                                   srcWidth:(CGFloat)srcWidth
                                  srcHeight:(CGFloat)srcHeight
                                   tileRect:(CGRect)tileRect {
  if (config.usesPanoramaPtzTiles) {
    return [self panoramaCropImageForConfig:config
                                      panDeg:tile.panDeg
                                      fovDeg:tile.fovDeg
                                 sourceImage:sourceImage
                                    srcWidth:srcWidth
                                   srcHeight:srcHeight
                                    tileRect:tileRect];
  }
  CIWarpKernel* kernel = self.ptzKernel;
  if (kernel == nil) return nil;
  CGFloat fovHRad = tile.fovDeg * M_PI / 180.0;
  CGFloat aspect = tileRect.size.height > 0 ? tileRect.size.width / tileRect.size.height : 1.0;
  CGFloat fovVRad = 2.0 * atan(tan(fovHRad / 2.0) / aspect);
  return [kernel applyWithExtent:tileRect
                      roiCallback:^CGRect(int index, CGRect destRect) {
                        return sourceImage.extent;
                      }
                       inputImage:sourceImage
                        arguments:@[
                          @(tileRect.origin.x), @(tileRect.origin.y), @(tileRect.size.width),
                          @(tileRect.size.height), @(srcWidth), @(srcHeight),
                          @(config.centerXNorm), @(config.centerYNorm), @(config.radiusNorm),
                          @(config.rotationDeg * M_PI / 180.0),
                          @(config.mountType == RTCDewarpMountTypeDesktop ? -1.0 : 1.0),
                          @(tile.panDeg * M_PI / 180.0), @(tile.tiltDeg * M_PI / 180.0),
                          @(tan(fovHRad / 2.0)), @(tan(fovVRad / 2.0))
                        ]];
}

/**
 * Renders a horizontally-scrollable crop of the same cylindrical panorama
 * projection used by -renderBaseTileForConfig:...'s ordinary
 * (non-pannable) path, instead of a fixed full-arc flatten or an
 * independent rectilinear virtual-PTZ camera: `fovDeg` degrees of azimuth
 * centered on `panDeg`, at the fixed kStripVerticalFovDeg vertical FOV. No
 * tilt parameter -- both the base tile and PTZ tile callers of this only
 * pan, matching the "scrub left/right through the overview, no up/down"
 * product requirement. `panDeg` is expected to be mutated live (see
 * RTCDewarpConfig.basePanDeg / RTCDewarpPtzTile.panDeg) as the user drags,
 * so this re-reads whatever the caller passes fresh every frame rather
 * than caching anything. Mirrors DewarpGlDrawer.drawPanoramaCrop on
 * Android exactly.
 */
- (nullable CIImage*)panoramaCropImageForConfig:(RTCDewarpConfig*)config
                                          panDeg:(float)panDeg
                                          fovDeg:(float)fovDeg
                                     sourceImage:(CIImage*)sourceImage
                                        srcWidth:(CGFloat)srcWidth
                                       srcHeight:(CGFloat)srcHeight
                                        tileRect:(CGRect)tileRect {
  CIWarpKernel* kernel = self.panoramaKernel;
  if (kernel == nil) return nil;
  CGFloat arcRad = fovDeg * M_PI / 180.0;
  CGFloat stripStartRad = (panDeg * M_PI / 180.0) - arcRad / 2.0;
  return [kernel applyWithExtent:tileRect
                      roiCallback:^CGRect(int index, CGRect destRect) {
                        return sourceImage.extent;
                      }
                       inputImage:sourceImage
                        arguments:@[
                          @(tileRect.origin.x), @(tileRect.origin.y), @(tileRect.size.width),
                          @(tileRect.size.height), @(srcWidth), @(srcHeight),
                          @(config.centerXNorm), @(config.centerYNorm), @(config.radiusNorm),
                          @(config.rotationDeg * M_PI / 180.0),
                          @(config.mountType == RTCDewarpMountTypeDesktop ? -1.0 : 1.0),
                          @(arcRad), @(stripStartRad),
                          @(tan(kStripVerticalFovDeg * M_PI / 180.0 / 2.0))
                        ]];
}

/**
 * Returns the tile's rect in `destPixelBuffer`'s absolute pixel coordinate
 * space (Core Image convention: origin bottom-left, y-up). Mirrors
 * DewarpGlDrawer.tileRectNormalizedTopLeft's proportions, converted from
 * that method's top-left/y-down normalized space into this one.
 */
- (CGRect)tileRectForIndex:(NSInteger)tileIndex
               ptzTileCount:(NSInteger)ptzTileCount
      baseTileHeightFraction:(CGFloat)baseTileHeightFraction
                  destWidth:(CGFloat)destWidth
                 destHeight:(CGFloat)destHeight {
  CGFloat xNorm, yNormTopDown, wNorm, hNorm;
  if (ptzTileCount == 0) {
    xNorm = 0;
    yNormTopDown = 0;
    wNorm = 1;
    hNorm = 1;
  } else if (tileIndex == 0) {
    xNorm = 0;
    yNormTopDown = 0;
    wNorm = 1;
    hNorm = baseTileHeightFraction;
  } else {
    NSInteger ptzIndex = tileIndex - 1;
    NSInteger cols = (NSInteger)ceil(sqrt((double)ptzTileCount));
    NSInteger rows = (NSInteger)ceil((double)ptzTileCount / (double)cols);
    NSInteger col = ptzIndex % cols;
    NSInteger row = ptzIndex / cols;
    CGFloat cellW = 1.0 / cols;
    CGFloat cellH = (1.0 - baseTileHeightFraction) / rows;
    xNorm = col * cellW;
    yNormTopDown = baseTileHeightFraction + row * cellH;
    wNorm = cellW;
    hNorm = cellH;
  }
  CGFloat tileWidth = MAX(1, wNorm * destWidth);
  CGFloat tileHeight = MAX(1, hNorm * destHeight);
  CGFloat tileX = xNorm * destWidth;
  // Flip from top-down normalized (yNormTopDown measured from the top) to
  // Core Image's bottom-left-origin absolute pixel space.
  CGFloat tileY = destHeight - (yNormTopDown * destHeight) - tileHeight;
  return CGRectMake(tileX, tileY, tileWidth, tileHeight);
}

+ (CGSize)compositeSizeForConfig:(RTCDewarpConfig*)config decodedSize:(CGSize)decodedSize {
  NSInteger ptzTileCount = config.ptzTileCount;
  if (ptzTileCount == 0) {
    if (config.usesPanoramaBase) {
      CGFloat aspect =
          config.panoramaArcSpanDeg / 90.0 / MAX(1, config.panoramaSplitCount);
      return CGSizeMake(decodedSize.width, MAX(1, decodedSize.width / aspect));
    }
    return CGSizeMake(decodedSize.width, decodedSize.width);  // raw circle: square canvas
  }
  return CGSizeMake(decodedSize.width,
                     MAX(1, decodedSize.width / (2.0 - config.baseTileHeightFraction)));
}

@end
