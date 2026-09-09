package com.cloudwebrtc.webrtc;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Fisheye lens dewarp configuration for a single {@link FlutterRTCVideoRenderer}.
 *
 * Mirrors the Dart-side {@code FisheyeDewarpConfig} payload
 * (lib/src/native/rtc_video_dewarp.dart) sent over the
 * "videoRendererSetDewarpConfig" method channel call. String values match
 * the Dart enums' {@code .name} exactly.
 */
public class DewarpConfig {

  public enum MountType {
    CEILING("ceiling"),
    DESKTOP("desktop"),
    WALL("wall");

    public final String wireName;

    MountType(String wireName) {
      this.wireName = wireName;
    }

    static MountType fromWireName(String wireName) {
      for (MountType v : values()) {
        if (v.wireName.equals(wireName)) return v;
      }
      throw new IllegalArgumentException("Unknown FisheyeMountType: " + wireName);
    }
  }

  /**
   * A display mode's base tile is either the raw fisheye circle
   * (panoramaArcSpanDeg == 0) or a panorama strip unwrapped from the given
   * arc span, optionally split into {@code panoramaSplitCount} stacked
   * strips (only meaningful when panoramaArcSpanDeg == 360; see
   * DewarpGlDrawer). {@code ptzTileCount} additional virtual-PTZ tiles are
   * composited alongside the base tile.
   */
  public enum DisplayMode {
    FISHEYE("fisheye", 0, 0, 0),
    PANO_180("pano180", 360, 2, 0),
    PANORAMIC("panoramic", 180, 1, 0),
    THREE_SIXTY_PLUS_1_PTZ("threeSixtyPlus1Ptz", 360, 1, 1),
    THREE_SIXTY_PLUS_6_PTZ("threeSixtyPlus6Ptz", 360, 1, 6),
    FISHEYE_PLUS_3_PTZ("fisheyePlus3Ptz", 0, 0, 3),
    FISHEYE_PLUS_4_PTZ("fisheyePlus4Ptz", 0, 0, 4),
    FISHEYE_PLUS_8_PTZ("fisheyePlus8Ptz", 0, 0, 8),
    PANO_PLUS_3_PTZ("panoPlus3Ptz", 180, 1, 3),
    PANO_PLUS_4_PTZ("panoPlus4Ptz", 180, 1, 4),
    PANO_PLUS_8_PTZ("panoPlus8Ptz", 180, 1, 8);

    public final String wireName;
    /** 0 means the base tile is the raw fisheye circle (no unwarp). */
    public final int panoramaArcSpanDeg;
    /** Only meaningful when panoramaArcSpanDeg > 0. */
    public final int panoramaSplitCount;
    public final int ptzTileCount;

    DisplayMode(String wireName, int panoramaArcSpanDeg, int panoramaSplitCount,
                int ptzTileCount) {
      this.wireName = wireName;
      this.panoramaArcSpanDeg = panoramaArcSpanDeg;
      this.panoramaSplitCount = panoramaSplitCount;
      this.ptzTileCount = ptzTileCount;
    }

    public boolean usesPanoramaBase() {
      return panoramaArcSpanDeg > 0;
    }

    /**
     * Fraction of the composite canvas height given to the base tile (the
     * rest goes to the PTZ tile grid). Placeholder pending pixel-accurate UI
     * mockups, same status as {@link DewarpGlDrawer}'s constants -- only
     * {@link #THREE_SIXTY_PLUS_1_PTZ} deviates from the shared 0.5 default
     * so far (product wants the overview strip to dominate: 2/3 overview,
     * 1/3 scrollable close-up).
     */
    public float baseTileHeightFraction() {
      return this == THREE_SIXTY_PLUS_1_PTZ ? 2f / 3f : 0.5f;
    }

    /**
     * Whether this mode's PTZ tile(s) are a scrollable crop of the same
     * cylindrical panorama projection as the base tile (pan-only, no
     * independent tilt/perspective) rather than an independent rectilinear
     * virtual-PTZ camera. Only {@link #THREE_SIXTY_PLUS_1_PTZ} does this so
     * far -- product wants its close-up to feel like "scrub through the same
     * overview", not a separate camera aimed somewhere.
     */
    public boolean usesPanoramaPtzTiles() {
      return this == THREE_SIXTY_PLUS_1_PTZ;
    }

    static DisplayMode fromWireName(String wireName) {
      for (DisplayMode v : values()) {
        if (v.wireName.equals(wireName)) return v;
      }
      throw new IllegalArgumentException("Unknown FisheyeDisplayMode: " + wireName);
    }
  }

  public static class PtzTile {
    /**
     * Mutable (unlike the rest of this class, which is a snapshot from the
     * last {@code videoRendererSetDewarpConfig} call): {@link
     * FlutterRTCVideoRenderer#updatePtzTilePan} mutates this field in place
     * on the main thread so a drag gesture can pan in real time without
     * paying for a full renderer release()/init() cycle every frame. {@link
     * DewarpGlDrawer} re-reads it fresh every frame on the render thread, so
     * no other synchronization is needed for this single-float field.
     */
    public volatile float panDeg;
    public final float tiltDeg;
    public final float fovDeg;

    public PtzTile(float panDeg, float tiltDeg, float fovDeg) {
      this.panDeg = panDeg;
      this.tiltDeg = tiltDeg;
      this.fovDeg = fovDeg;
    }
  }

  public final MountType mountType;
  public final DisplayMode displayMode;
  public final float centerXNorm;
  public final float centerYNorm;
  public final float radiusNorm;
  public final float rotationDeg;
  public final List<PtzTile> ptzTiles;

  /**
   * Independent pan for the base tile, only meaningful when {@link
   * DisplayMode#usesPanoramaPtzTiles()} is true: in that case the base tile
   * is *also* a pannable crop of the panorama (same FOV as {@code
   * ptzTiles.get(0)}, per product's "both windows default to the same
   * zoom, but pan independently" requirement), not a single fixed
   * full-arc flatten. Mutable/volatile for the same drag-in-real-time
   * reason as {@link PtzTile#panDeg}; defaults to 0 (unrotated).
   */
  public volatile float basePanDeg;

  public DewarpConfig(MountType mountType, DisplayMode displayMode, float centerXNorm,
                       float centerYNorm, float radiusNorm, float rotationDeg,
                       List<PtzTile> ptzTiles) {
    this.mountType = mountType;
    this.displayMode = displayMode;
    this.centerXNorm = centerXNorm;
    this.centerYNorm = centerYNorm;
    this.radiusNorm = radiusNorm;
    this.rotationDeg = rotationDeg;
    this.ptzTiles = ptzTiles;
    this.basePanDeg = 0f;
  }

  /**
   * Number of grid tiles this config renders: the base (fisheye/panorama)
   * tile plus every virtual PTZ tile.
   */
  public int totalTileCount() {
    return 1 + ptzTiles.size();
  }

  @SuppressWarnings("unchecked")
  public static DewarpConfig fromMap(Map<String, Object> args) {
    MountType mountType = MountType.fromWireName((String) args.get("mountType"));
    DisplayMode displayMode = DisplayMode.fromWireName((String) args.get("displayMode"));

    Map<String, Object> calibration = (Map<String, Object>) args.get("calibration");
    float centerXNorm = toFloat(calibration.get("centerXNorm"), 0.5f);
    float centerYNorm = toFloat(calibration.get("centerYNorm"), 0.5f);
    float radiusNorm = toFloat(calibration.get("radiusNorm"), 0.5f);
    float rotationDeg = toFloat(calibration.get("rotationDeg"), 0f);

    List<PtzTile> ptzTiles = new ArrayList<>();
    List<Object> rawTiles = (List<Object>) args.get("ptzTiles");
    if (rawTiles != null) {
      for (Object rawTile : rawTiles) {
        Map<String, Object> tile = (Map<String, Object>) rawTile;
        ptzTiles.add(new PtzTile(
                toFloat(tile.get("panDeg"), 0f),
                toFloat(tile.get("tiltDeg"), 0f),
                toFloat(tile.get("fovDeg"), 90f)));
      }
    }

    if (ptzTiles.size() != displayMode.ptzTileCount) {
      throw new IllegalArgumentException(
              displayMode.wireName + " requires " + displayMode.ptzTileCount
                      + " ptzTiles, got " + ptzTiles.size());
    }

    return new DewarpConfig(mountType, displayMode, centerXNorm, centerYNorm, radiusNorm,
            rotationDeg, ptzTiles);
  }

  private static float toFloat(Object value, float defaultValue) {
    if (value == null) return defaultValue;
    return ((Number) value).floatValue();
  }
}
