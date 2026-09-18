#import <CoreVideo/CoreVideo.h>
#import <Foundation/Foundation.h>

#import "RTCDewarpConfig.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Renders a fisheye-lens video frame through an RTCDewarpConfig using Core
 * Image, into a separate destination CVPixelBuffer sized to
 * +compositeSizeForConfig:decodedSize:.
 *
 * See DewarpGlDrawer.java (the Android counterpart) for the shared
 * projection math this mirrors -- an equidistant fisheye lens model
 * composed from the same three primitives: raw fisheye passthrough,
 * cylindrical panorama unwrap, and rectilinear virtual-PTZ crop.
 */
@interface RTCDewarpProcessor : NSObject

/**
 * Renders `config` against `sourcePixelBuffer`, writing the composited
 * tile grid into `destPixelBuffer`. `destPixelBuffer` must already be
 * sized to +compositeSizeForConfig:decodedSize: for
 * (sourcePixelBuffer's width, height).
 */
- (void)renderConfig:(RTCDewarpConfig*)config
    sourcePixelBuffer:(CVPixelBufferRef)sourcePixelBuffer
      destPixelBuffer:(CVPixelBufferRef)destPixelBuffer;

/**
 * The composite canvas size `config` wants for a decoded frame of
 * `decodedSize` -- used to size the destination pixel buffer so the tile
 * grid isn't stretched. Placeholder proportions pending pixel-accurate UI
 * mockups; mirrors DewarpGlDrawer.compositeSize on Android exactly.
 */
+ (CGSize)compositeSizeForConfig:(RTCDewarpConfig*)config decodedSize:(CGSize)decodedSize;

@end

NS_ASSUME_NONNULL_END
