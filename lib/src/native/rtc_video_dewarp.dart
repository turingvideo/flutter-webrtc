/// Fisheye lens dewarping configuration for [RTCVideoRenderer].
///
/// This is a native-only (Android/iOS/macOS) feature: it post-processes
/// already-decoded video frames with a GPU shader before they reach the
/// Flutter texture, so it has no meaning on web/other platforms and
/// [RTCVideoRenderer.setDewarpConfig] is a no-op there.
library;

/// Physical installation of the fisheye lens. Determines which
/// [FisheyeDisplayMode]s are valid (see [kValidFisheyeDisplayModes]) and how
/// the projection math orients the source image.
enum FisheyeMountType { ceiling, desktop, wall }

/// How a dewarped fisheye stream should be laid out on screen.
///
/// Not every mode is valid for every [FisheyeMountType] — a wall-mounted
/// lens only ever sees a ~180° hemisphere, so the full-360° modes
/// ([threeSixtyPlus1Ptz], [threeSixtyPlus6Ptz]) and the dual-strip
/// [pano180] mode don't apply to it. Use [kValidFisheyeDisplayModes] (or
/// [isFisheyeDisplayModeValid]) to filter UI options per mount type.
enum FisheyeDisplayMode {
  /// Raw circular fisheye image, no correction.
  fisheye,

  /// Ceiling/desktop only: the full 360° circle unwrapped into two stacked
  /// 180° panoramic strips.
  pano180,

  /// Wall only: the ~180° hemisphere unwrapped into a single panoramic
  /// strip.
  panoramic,

  /// Ceiling/desktop only: 360° panorama strip + 1 virtual PTZ view.
  threeSixtyPlus1Ptz,

  /// Ceiling/desktop only: 360° panorama strip + 6 virtual PTZ views.
  threeSixtyPlus6Ptz,

  /// Ceiling/desktop only: raw fisheye circle + 3 virtual PTZ views.
  fisheyePlus3Ptz,

  /// Ceiling/desktop only: raw fisheye circle + 4 virtual PTZ views.
  fisheyePlus4Ptz,

  /// Ceiling/desktop only: raw fisheye circle + 8 virtual PTZ views.
  fisheyePlus8Ptz,

  /// Wall only: panoramic strip + 3 virtual PTZ views.
  panoPlus3Ptz,

  /// Wall only: panoramic strip + 4 virtual PTZ views.
  panoPlus4Ptz,

  /// Wall only: panoramic strip + 8 virtual PTZ views.
  panoPlus8Ptz,
}

/// Which [FisheyeDisplayMode]s are selectable for each [FisheyeMountType].
///
/// Ceiling and desktop mounts see the full 360° circle (7 modes); wall
/// mounts only see a ~180° hemisphere (5 modes, no full-360 or dual-strip
/// variants).
const Map<FisheyeMountType, Set<FisheyeDisplayMode>>
    kValidFisheyeDisplayModes = {
  FisheyeMountType.ceiling: {
    FisheyeDisplayMode.fisheye,
    FisheyeDisplayMode.pano180,
    FisheyeDisplayMode.threeSixtyPlus1Ptz,
    FisheyeDisplayMode.threeSixtyPlus6Ptz,
    FisheyeDisplayMode.fisheyePlus3Ptz,
    FisheyeDisplayMode.fisheyePlus4Ptz,
    FisheyeDisplayMode.fisheyePlus8Ptz,
  },
  FisheyeMountType.desktop: {
    FisheyeDisplayMode.fisheye,
    FisheyeDisplayMode.pano180,
    FisheyeDisplayMode.threeSixtyPlus1Ptz,
    FisheyeDisplayMode.threeSixtyPlus6Ptz,
    FisheyeDisplayMode.fisheyePlus3Ptz,
    FisheyeDisplayMode.fisheyePlus4Ptz,
    FisheyeDisplayMode.fisheyePlus8Ptz,
  },
  FisheyeMountType.wall: {
    FisheyeDisplayMode.fisheye,
    FisheyeDisplayMode.panoramic,
    FisheyeDisplayMode.panoPlus3Ptz,
    FisheyeDisplayMode.panoPlus4Ptz,
    FisheyeDisplayMode.panoPlus8Ptz,
  },
};

/// Whether [mode] is selectable when the lens is mounted as [mountType].
bool isFisheyeDisplayModeValid(
        FisheyeMountType mountType, FisheyeDisplayMode mode) =>
    kValidFisheyeDisplayModes[mountType]!.contains(mode);

/// The number of independent virtual PTZ tiles a [FisheyeDisplayMode]
/// composites alongside its base (fisheye/panorama) tile.
int fisheyePtzTileCount(FisheyeDisplayMode mode) {
  switch (mode) {
    case FisheyeDisplayMode.fisheye:
    case FisheyeDisplayMode.pano180:
    case FisheyeDisplayMode.panoramic:
      return 0;
    case FisheyeDisplayMode.threeSixtyPlus1Ptz:
      return 1;
    case FisheyeDisplayMode.fisheyePlus3Ptz:
    case FisheyeDisplayMode.panoPlus3Ptz:
      return 3;
    case FisheyeDisplayMode.fisheyePlus4Ptz:
    case FisheyeDisplayMode.panoPlus4Ptz:
      return 4;
    case FisheyeDisplayMode.threeSixtyPlus6Ptz:
      return 6;
    case FisheyeDisplayMode.fisheyePlus8Ptz:
    case FisheyeDisplayMode.panoPlus8Ptz:
      return 8;
  }
}

/// Where the fisheye circle sits within the decoded frame, and the lens'
/// mounting rotation.
///
/// There is no on-device auto-detection for this — [centerXNorm],
/// [centerYNorm] and [radiusNorm] must be supplied by the app (e.g. from a
/// one-time per-camera calibration step where the user aligns a draggable
/// circle overlay on top of the raw [FisheyeDisplayMode.fisheye] stream).
class FisheyeCalibration {
  const FisheyeCalibration({
    this.centerXNorm = 0.5,
    this.centerYNorm = 0.5,
    this.radiusNorm = 0.5,
    this.rotationDeg = 0,
  });

  /// Fisheye circle center X, normalized to the decoded frame width [0, 1].
  /// Measured left-to-right, matching a Flutter overlay's natural
  /// coordinate system.
  final double centerXNorm;

  /// Fisheye circle center Y, normalized to the decoded frame height [0, 1].
  /// Measured top-to-bottom (0 = top of the displayed frame), matching a
  /// Flutter overlay's natural coordinate system -- native renderers
  /// convert this to their own texture-sampling convention internally.
  final double centerYNorm;

  /// Fisheye circle radius, normalized to min(frame width, frame height).
  final double radiusNorm;

  /// Mounting rotation offset, in degrees.
  final double rotationDeg;

  Map<String, dynamic> toMap() => {
        'centerXNorm': centerXNorm,
        'centerYNorm': centerYNorm,
        'radiusNorm': radiusNorm,
        'rotationDeg': rotationDeg,
      };
}

/// Pan/tilt/zoom of a single virtual PTZ tile carved out of the fisheye
/// image.
class PtzTileConfig {
  const PtzTileConfig({
    required this.panDeg,
    required this.tiltDeg,
    required this.fovDeg,
  });

  /// Azimuth, in degrees.
  final double panDeg;

  /// Elevation, in degrees.
  final double tiltDeg;

  /// Horizontal field of view ("zoom"), in degrees.
  final double fovDeg;

  Map<String, dynamic> toMap() => {
        'panDeg': panDeg,
        'tiltDeg': tiltDeg,
        'fovDeg': fovDeg,
      };
}

/// Full fisheye dewarp configuration for a single [RTCVideoRenderer].
class FisheyeDewarpConfig {
  FisheyeDewarpConfig({
    required this.mountType,
    required this.displayMode,
    this.calibration = const FisheyeCalibration(),
    List<PtzTileConfig>? ptzTiles,
  }) : ptzTiles = ptzTiles ?? const [] {
    assert(
      isFisheyeDisplayModeValid(mountType, displayMode),
      '$displayMode is not valid for mount type $mountType',
    );
    assert(
      this.ptzTiles.length == fisheyePtzTileCount(displayMode),
      '$displayMode requires ${fisheyePtzTileCount(displayMode)} ptzTiles, '
      'got ${this.ptzTiles.length}',
    );
  }

  final FisheyeMountType mountType;
  final FisheyeDisplayMode displayMode;
  final FisheyeCalibration calibration;
  final List<PtzTileConfig> ptzTiles;

  Map<String, dynamic> toMap() => {
        'mountType': mountType.name,
        'displayMode': displayMode.name,
        'calibration': calibration.toMap(),
        'ptzTiles': ptzTiles.map((t) => t.toMap()).toList(),
      };
}
