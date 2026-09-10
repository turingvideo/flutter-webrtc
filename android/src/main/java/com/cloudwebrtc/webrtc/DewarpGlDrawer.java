package com.cloudwebrtc.webrtc;

import static android.opengl.GLES20.GL_COLOR_ATTACHMENT0;
import static android.opengl.GLES20.GL_FRAMEBUFFER;
import static android.opengl.GLES20.GL_RGBA;
import static android.opengl.GLES20.GL_TEXTURE0;
import static android.opengl.GLES20.GL_TEXTURE1;
import static android.opengl.GLES20.GL_TEXTURE2;
import static android.opengl.GLES20.GL_TEXTURE_2D;
import static android.opengl.GLES20.GL_TRIANGLE_STRIP;
import static android.opengl.GLES20.glActiveTexture;
import static android.opengl.GLES20.glBindFramebuffer;
import static android.opengl.GLES20.glBindTexture;
import static android.opengl.GLES20.glDrawArrays;
import static android.opengl.GLES20.glFramebufferTexture2D;
import static android.opengl.GLES20.glUniform1f;
import static android.opengl.GLES20.glUniform1i;
import static android.opengl.GLES20.glUniform2f;
import static android.opengl.GLES20.glUniformMatrix4fv;
import static android.opengl.GLES20.glViewport;
import static android.opengl.GLES11Ext.GL_TEXTURE_EXTERNAL_OES;

import org.webrtc.GlShader;
import org.webrtc.GlTextureFrameBuffer;
import org.webrtc.GlUtil;
import org.webrtc.RendererCommon;

import java.nio.FloatBuffer;
import java.util.List;

/**
 * {@link RendererCommon.GlDrawer} that renders a fisheye-lens video frame
 * through the configured {@link DewarpConfig} instead of drawing it
 * as-is (which is what the stock {@code org.webrtc.GlRectDrawer} does).
 *
 * Rendering is split into two GL passes per frame:
 *  1. "Blit" pass: whatever source the decoder handed us (an OES texture
 *     for hardware-decoded frames, 3 YUV plane textures for
 *     software-decoded frames, or a plain RGB texture) is drawn, with its
 *     texture-matrix correction applied, into an offscreen
 *     {@link GlTextureFrameBuffer} as a plain upright RGBA texture. This
 *     isolates every dewarp shader below from decoder-specific texture
 *     formats/orientation.
 *  2. "Composite" pass: for each output tile (the base fisheye/panorama
 *     tile, plus one per configured PTZ view), {@code glViewport} is set
 *     to that tile's rectangle of the destination surface and a full-quad
 *     draw call samples the normalized texture from pass 1 through the
 *     matching projection shader (see PRIMITIVE_*_FRAGMENT_SHADER below).
 *
 * The projection math implements an equidistant fisheye lens model
 * (r = f*theta); see the "投影数学模型" section of the fisheye dewarp plan
 * for the derivation. Tile grid proportions ({@link
 * DewarpConfig.DisplayMode#baseTileHeightFraction()}) are a placeholder
 * pending pixel-accurate UI mockups.
 */
public class DewarpGlDrawer implements RendererCommon.GlDrawer {

  private static final String VERTEX_ATTRIB_POSITION = "in_pos";
  private static final String VERTEX_ATTRIB_TEXCOORD = "in_tc";

  // --- Pass 1 ("blit") shaders: decoder-format-specific, apply texMatrix. ---

  private static final String BLIT_VERTEX_SHADER =
      "varying vec2 v_tc;\n"
          + "attribute vec4 in_pos;\n"
          + "attribute vec4 in_tc;\n"
          + "uniform mat4 texMatrix;\n"
          + "void main() {\n"
          + "  gl_Position = in_pos;\n"
          + "  v_tc = (texMatrix * in_tc).xy;\n"
          + "}\n";

  private static final String BLIT_OES_FRAGMENT_SHADER =
      "#extension GL_OES_EGL_image_external : require\n"
          + "precision mediump float;\n"
          + "varying vec2 v_tc;\n"
          + "uniform samplerExternalOES tex;\n"
          + "void main() {\n"
          + "  gl_FragColor = texture2D(tex, v_tc);\n"
          + "}\n";

  private static final String BLIT_RGB_FRAGMENT_SHADER =
      "precision mediump float;\n"
          + "varying vec2 v_tc;\n"
          + "uniform sampler2D tex;\n"
          + "void main() {\n"
          + "  gl_FragColor = texture2D(tex, v_tc);\n"
          + "}\n";

  // Standard BT.601 full-range YUV -> RGB.
  private static final String BLIT_YUV_FRAGMENT_SHADER =
      "precision mediump float;\n"
          + "varying vec2 v_tc;\n"
          + "uniform sampler2D y_tex;\n"
          + "uniform sampler2D u_tex;\n"
          + "uniform sampler2D v_tex;\n"
          + "void main() {\n"
          + "  float y = texture2D(y_tex, v_tc).r;\n"
          + "  float u = texture2D(u_tex, v_tc).r - 0.5;\n"
          + "  float v = texture2D(v_tex, v_tc).r - 0.5;\n"
          + "  gl_FragColor = vec4(y + 1.403 * v, y - 0.344 * u - 0.714 * v,\n"
          + "                      y + 1.770 * u, 1.0);\n"
          + "}\n";

  // --- Pass 2 ("composite") shaders: sample the normalized texture from
  // pass 1 through the equidistant fisheye projection math. No texMatrix;
  // v_tc is already a plain [0,1] destination-tile coordinate. ---

  private static final String COMPOSITE_VERTEX_SHADER =
      "varying vec2 v_tc;\n"
          + "attribute vec4 in_pos;\n"
          + "attribute vec4 in_tc;\n"
          + "void main() {\n"
          + "  gl_Position = in_pos;\n"
          + "  v_tc = in_tc.xy;\n"
          + "}\n";

  private static final String FISHEYE_SAMPLE_FUNCTION =
      "uniform sampler2D sourceTex;\n"
          // center/radius are calibrated top-down (y=0 at the top of the
          // visually displayed frame, matching how a Flutter calibration
          // overlay naturally measures), but GL texture coordinates are
          // bottom-up (texcoord.y=0 is the bottom of the upright image,
          // per FULL_RECTANGLE_BUFFER/FULL_RECTANGLE_TEXCOORD_BUFFER's
          // pairing below) -- fisheyeSample() flips between the two.
          + "uniform vec2 center;\n"
          + "uniform float radius;\n"
          + "uniform float rotationOffsetRad;\n"
          + "uniform float verticalFlipSign;\n"
          + "const float THETA_MAX = 1.5707963268;\n" // 90 degrees
          + "vec4 fisheyeSample(float theta, float phi) {\n"
          + "  if (theta > THETA_MAX) return vec4(0.0, 0.0, 0.0, 1.0);\n"
          + "  float rNorm = theta / THETA_MAX;\n"
          + "  float texXTopDown = center.x + rNorm * radius * cos(phi + rotationOffsetRad);\n"
          + "  float texYTopDown = center.y\n"
          + "      + rNorm * radius * sin(phi + rotationOffsetRad) * verticalFlipSign;\n"
          + "  if (texXTopDown < 0.0 || texXTopDown > 1.0 || texYTopDown < 0.0\n"
          + "      || texYTopDown > 1.0) {\n"
          + "    return vec4(0.0, 0.0, 0.0, 1.0);\n"
          + "  }\n"
          + "  return texture2D(sourceTex, vec2(texXTopDown, 1.0 - texYTopDown));\n"
          + "}\n";

  /** Primitive A: raw fisheye circle, no correction. */
  private static final String PRIMITIVE_A_FRAGMENT_SHADER =
      "precision highp float;\n"
          + "varying vec2 v_tc;\n"
          + "uniform sampler2D sourceTex;\n"
          + "void main() {\n"
          + "  gl_FragColor = texture2D(sourceTex, v_tc);\n"
          + "}\n";

  /**
   * Primitive B: cylindrical panorama unwrap of one strip spanning
   * [stripStartRad, stripStartRad + arcPerStripRad).
   *
   * Horizontally this sweeps azimuth linearly (phi), same as a naive
   * equirectangular unwrap would. Vertically it does NOT map linearly to
   * theta -- that would put the horizon at a fixed theta (matching the
   * lens' own angular spacing) rather than at the tile's edge, and would
   * vertically curve/compress objects standing near the strip's edges.
   * Real fisheye-camera "panorama" dewarp modes instead treat each column
   * as its own zero-width rectilinear ("pushbroom") camera pointed at the
   * horizon, so verticals stay straight: STRIP_VERTICAL_FOV_RAD is that
   * per-column camera's vertical FOV, and v=1.0 (top of tile) always lands
   * exactly on the horizon (theta=THETA_MAX) regardless of mount, with
   * theta decreasing toward the lens' own zenith/nadir as v decreases to
   * 0.0. (Derivation: a column's local ray is (0, ndcV*halfTan, 1) in a
   * frame whose forward axis IS the horizon direction; rotating that frame
   * back to the fisheye's own boresight-relative (theta,phi) via a fixed
   * 90° tilt + a phi-rotation simplifies to theta = THETA_MAX - atan(ndcV *
   * halfTan) -- see the fisheye dewarp plan for the full derivation. ndcV
   * only spans [0,1], not [-1,1]: theta can never legitimately exceed
   * THETA_MAX -- that's past the lens' own edge -- so the other half of a
   * symmetric [-1,1] sweep would just be the fisheyeSample() out-of-bounds
   * black fallback below, wasting half the tile's height on a black band.)
   * STRIP_VERTICAL_FOV_RAD is a placeholder pending real product tuning,
   * same status as {@link DewarpConfig.DisplayMode#baseTileHeightFraction()}
   * above. Also reused, with a narrower arcPerStripRad, by {@link
   * #drawPanoramaCropPtzTile} for modes where {@link
   * DewarpConfig.DisplayMode#usesPanoramaPtzTiles()} is true.
   */
  private static final String PRIMITIVE_B_FRAGMENT_SHADER =
      "precision highp float;\n"
          + "varying vec2 v_tc;\n"
          + "uniform float arcPerStripRad;\n"
          + "uniform float stripStartRad;\n"
          + "uniform float halfTanStripVFov;\n"
          + FISHEYE_SAMPLE_FUNCTION
          + "void main() {\n"
          + "  float ndcV = 1.0 - v_tc.y;\n"
          + "  float theta = THETA_MAX - atan(ndcV * halfTanStripVFov);\n"
          + "  float phi = stripStartRad + v_tc.x * arcPerStripRad;\n"
          + "  gl_FragColor = fisheyeSample(theta, phi);\n"
          + "}\n";

  /** Primitive C: rectilinear (perspective) virtual PTZ crop. */
  private static final String PRIMITIVE_C_FRAGMENT_SHADER =
      "precision highp float;\n"
          + "varying vec2 v_tc;\n"
          + "uniform float panRad;\n"
          + "uniform float tiltRad;\n"
          + "uniform float halfTanFovH;\n"
          + "uniform float halfTanFovV;\n"
          + FISHEYE_SAMPLE_FUNCTION
          + "void main() {\n"
          + "  float ndcX = v_tc.x * 2.0 - 1.0;\n"
          + "  float ndcY = v_tc.y * 2.0 - 1.0;\n"
          + "  vec3 dirLocal = normalize(vec3(ndcX * halfTanFovH, ndcY * halfTanFovV, 1.0));\n"
          + "  float ct = cos(tiltRad), st = sin(tiltRad);\n"
          + "  vec3 afterTilt = vec3(dirLocal.x, dirLocal.y * ct - dirLocal.z * st,\n"
          + "                        dirLocal.y * st + dirLocal.z * ct);\n"
          + "  float cp = cos(panRad), sp = sin(panRad);\n"
          + "  vec3 dirWorld = vec3(afterTilt.x * cp - afterTilt.y * sp,\n"
          + "                       afterTilt.x * sp + afterTilt.y * cp, afterTilt.z);\n"
          + "  float theta = acos(clamp(dirWorld.z, -1.0, 1.0));\n"
          + "  float phi = atan(dirWorld.y, dirWorld.x);\n"
          + "  gl_FragColor = fisheyeSample(theta, phi);\n"
          + "}\n";

  /**
   * Primitive D: plain rectangular crop of the raw fisheye circle -- pan
   * and tilt an offset window around {@code center}, no theta/phi
   * reprojection at all (unlike Primitives B/C, which both treat the
   * destination as a ray into the scene). A square crop inevitably has
   * corners farther from its center than the inscribed circle's radius, so
   * a naive crop shows the real black margin around the lens' circular
   * image in those corners. Instead of sampling that black margin as-is,
   * any point that would land outside the circle is radially clamped back
   * onto the circle's edge (same direction from {@code center}, distance
   * capped at {@code radius}) before sampling -- this stretches the rim of
   * the actual image outward to fill the corners, matching the same
   * "never black, stretch instead" rule already applied to the texture-
   * bounds clamp below for panning/zooming near the edge. center/radius
   * are calibrated top-down (see FISHEYE_SAMPLE_FUNCTION's doc); this
   * shader works directly in that space and does its own top-down-to-GL-
   * bottom-up flip at the end, so it doesn't reuse fisheyeSample() (which
   * also does per-pixel theta-based invalidation this primitive has no use
   * for).
   */
  private static final String PRIMITIVE_D_FRAGMENT_SHADER =
      "precision highp float;\n"
          + "varying vec2 v_tc;\n"
          + "uniform sampler2D sourceTex;\n"
          + "uniform vec2 center;\n"
          + "uniform float radius;\n"
          + "uniform float verticalFlipSign;\n"
          + "uniform vec2 cropOffsetNorm;\n"
          + "uniform float cropHalfSizeNorm;\n"
          + "void main() {\n"
          + "  vec2 ndc = v_tc * 2.0 - 1.0;\n"
          + "  vec2 texTopDown = center + cropOffsetNorm * radius\n"
          + "      + ndc * cropHalfSizeNorm * radius * vec2(1.0, verticalFlipSign);\n"
          + "  vec2 rel = texTopDown - center;\n"
          + "  float dist = length(rel);\n"
          + "  if (dist > radius) {\n"
          + "    texTopDown = center + rel * (radius / dist);\n"
          + "  }\n"
          + "  texTopDown = clamp(texTopDown, vec2(0.0), vec2(1.0));\n"
          + "  gl_FragColor = texture2D(sourceTex, vec2(texTopDown.x, 1.0 - texTopDown.y));\n"
          + "}\n";

  // Full [-1,1] quad, drawn with GL_TRIANGLE_STRIP; texcoord doubles as the
  // per-tile destination coordinate in [0,1] consumed by every shader above.
  private static final FloatBuffer FULL_RECTANGLE_BUFFER =
      GlUtil.createFloatBuffer(new float[] {-1f, -1f, 1f, -1f, -1f, 1f, 1f, 1f});
  private static final FloatBuffer FULL_RECTANGLE_TEXCOORD_BUFFER =
      GlUtil.createFloatBuffer(new float[] {0f, 0f, 1f, 0f, 0f, 1f, 1f, 1f});

  // Vertical field of view of Primitive B's per-column "pushbroom" camera
  // (see PRIMITIVE_B_FRAGMENT_SHADER's doc). Placeholder pending real
  // product tuning, same status as
  // DewarpConfig.DisplayMode.baseTileHeightFraction() above.
  private static final float STRIP_VERTICAL_FOV_DEG = 100f;

  private final DewarpConfig config;

  private GlShader blitOesShader;
  private GlShader blitRgbShader;
  private GlShader blitYuvShader;
  private GlShader primitiveAShader;
  private GlShader primitiveBShader;
  private GlShader primitiveCShader;
  private GlShader primitiveDShader;
  private GlTextureFrameBuffer normalizedFrameBuffer;

  public DewarpGlDrawer(DewarpConfig config) {
    this.config = config;
  }

  @Override
  public void drawOes(int oesTextureId, float[] texMatrix, int frameWidth, int frameHeight,
                       int viewportX, int viewportY, int viewportWidth, int viewportHeight) {
    ensureNormalizedFrameBuffer(frameWidth, frameHeight);
    if (blitOesShader == null) {
      blitOesShader = new GlShader(BLIT_VERTEX_SHADER, BLIT_OES_FRAGMENT_SHADER);
    }
    bindNormalizedFrameBufferForWriting(frameWidth, frameHeight);
    blitOesShader.useProgram();
    GlUtil.checkNoGLES2Error("DewarpGlDrawer.drawOes.useProgram");
    glUniformMatrix4fv(blitOesShader.getUniformLocation("texMatrix"), 1, false, texMatrix, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, oesTextureId);
    glUniform1i(blitOesShader.getUniformLocation("tex"), 0);
    drawFullQuad(blitOesShader);
    composite(viewportX, viewportY, viewportWidth, viewportHeight);
  }

  @Override
  public void drawYuv(int[] yuvTextures, float[] texMatrix, int frameWidth, int frameHeight,
                       int viewportX, int viewportY, int viewportWidth, int viewportHeight) {
    ensureNormalizedFrameBuffer(frameWidth, frameHeight);
    if (blitYuvShader == null) {
      blitYuvShader = new GlShader(BLIT_VERTEX_SHADER, BLIT_YUV_FRAGMENT_SHADER);
    }
    bindNormalizedFrameBufferForWriting(frameWidth, frameHeight);
    blitYuvShader.useProgram();
    glUniformMatrix4fv(blitYuvShader.getUniformLocation("texMatrix"), 1, false, texMatrix, 0);
    int[] units = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2};
    String[] names = {"y_tex", "u_tex", "v_tex"};
    for (int i = 0; i < 3; i++) {
      glActiveTexture(units[i]);
      glBindTexture(GL_TEXTURE_2D, yuvTextures[i]);
      glUniform1i(blitYuvShader.getUniformLocation(names[i]), i);
    }
    drawFullQuad(blitYuvShader);
    composite(viewportX, viewportY, viewportWidth, viewportHeight);
  }

  @Override
  public void drawRgb(int textureId, float[] texMatrix, int frameWidth, int frameHeight,
                       int viewportX, int viewportY, int viewportWidth, int viewportHeight) {
    ensureNormalizedFrameBuffer(frameWidth, frameHeight);
    if (blitRgbShader == null) {
      blitRgbShader = new GlShader(BLIT_VERTEX_SHADER, BLIT_RGB_FRAGMENT_SHADER);
    }
    bindNormalizedFrameBufferForWriting(frameWidth, frameHeight);
    blitRgbShader.useProgram();
    glUniformMatrix4fv(blitRgbShader.getUniformLocation("texMatrix"), 1, false, texMatrix, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(blitRgbShader.getUniformLocation("tex"), 0);
    drawFullQuad(blitRgbShader);
    composite(viewportX, viewportY, viewportWidth, viewportHeight);
  }

  @Override
  public void release() {
    if (blitOesShader != null) blitOesShader.release();
    if (blitRgbShader != null) blitRgbShader.release();
    if (blitYuvShader != null) blitYuvShader.release();
    if (primitiveAShader != null) primitiveAShader.release();
    if (primitiveBShader != null) primitiveBShader.release();
    if (primitiveCShader != null) primitiveCShader.release();
    if (primitiveDShader != null) primitiveDShader.release();
    if (normalizedFrameBuffer != null) normalizedFrameBuffer.release();
    blitOesShader = null;
    blitRgbShader = null;
    blitYuvShader = null;
    primitiveAShader = null;
    primitiveBShader = null;
    primitiveCShader = null;
    primitiveDShader = null;
    normalizedFrameBuffer = null;
  }

  private void ensureNormalizedFrameBuffer(int width, int height) {
    if (normalizedFrameBuffer == null) {
      normalizedFrameBuffer = new GlTextureFrameBuffer(GL_RGBA);
    }
    normalizedFrameBuffer.setSize(width, height);
  }

  private void bindNormalizedFrameBufferForWriting(int width, int height) {
    glBindFramebuffer(GL_FRAMEBUFFER, normalizedFrameBuffer.getFrameBufferId());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        normalizedFrameBuffer.getTextureId(), 0);
    glViewport(0, 0, width, height);
  }

  /** Draws each output tile for {@link #config} into the real destination surface. */
  private void composite(int viewportX, int viewportY, int viewportWidth, int viewportHeight) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    int ptzTileCount = config.displayMode.ptzTileCount;
    int totalTiles = 1 + ptzTileCount;
    float baseTileHeightFraction = config.displayMode.baseTileHeightFraction();
    for (int tileIndex = 0; tileIndex < totalTiles; tileIndex++) {
      float[] rect = tileRectNormalizedTopLeft(tileIndex, ptzTileCount, baseTileHeightFraction);
      int tileX = viewportX + (int) (rect[0] * viewportWidth);
      int tileYTopLeft = (int) (rect[1] * viewportHeight);
      int tileW = Math.max(1, (int) (rect[2] * viewportWidth));
      int tileH = Math.max(1, (int) (rect[3] * viewportHeight));
      int tileYGl = viewportY + viewportHeight - tileYTopLeft - tileH;
      glViewport(tileX, tileYGl, tileW, tileH);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, normalizedFrameBuffer.getTextureId());

      if (tileIndex == 0) {
        drawBaseTile(tileX, tileYGl, tileW, tileH);
      } else {
        drawPtzTile(config.ptzTiles.get(tileIndex - 1), tileW, tileH);
      }
    }
  }

  private void drawBaseTile(int tileX, int tileYGl, int tileW, int tileH) {
    if (config.displayMode.usesDirectCropBase()) {
      // Same default zoom as the PTZ tile below it, per product's "both
      // windows default to the same zoom" requirement -- see
      // DewarpConfig.basePanDeg/baseTiltDeg's doc.
      float fovDeg = config.ptzTiles.isEmpty() ? 90f : config.ptzTiles.get(0).fovDeg;
      drawDirectCircleCrop(config.basePanDeg, config.baseTiltDeg, fovDeg);
      return;
    }
    if (!config.displayMode.usesPanoramaBase()) {
      if (primitiveAShader == null) {
        primitiveAShader = new GlShader(COMPOSITE_VERTEX_SHADER, PRIMITIVE_A_FRAGMENT_SHADER);
      }
      primitiveAShader.useProgram();
      drawFullQuad(primitiveAShader);
      return;
    }
    if (primitiveBShader == null) {
      primitiveBShader = new GlShader(COMPOSITE_VERTEX_SHADER, PRIMITIVE_B_FRAGMENT_SHADER);
    }
    float arcPerStripRad =
        (float) Math.toRadians(
            config.displayMode.panoramaArcSpanDeg / (float) config.displayMode.panoramaSplitCount);
    int splitCount = config.displayMode.panoramaSplitCount;
    // splitCount == 1: a single strip fills the whole base tile viewport.
    // splitCount == 2 (the "180°Pano"/pano180 mode): the two 180° strips
    // that together cover the full 360° circle are stacked as the top and
    // bottom halves of the base tile, each gets its own draw call into its
    // own half-height sub-viewport.
    int stripHeight = Math.max(1, tileH / splitCount);
    for (int strip = 0; strip < splitCount; strip++) {
      int stripYGl = tileYGl + tileH - (strip + 1) * stripHeight;
      glViewport(tileX, stripYGl, tileW, stripHeight);
      primitiveBShader.useProgram();
      setFisheyeSampleUniforms(primitiveBShader);
      glUniform1f(primitiveBShader.getUniformLocation("arcPerStripRad"), arcPerStripRad);
      glUniform1f(primitiveBShader.getUniformLocation("stripStartRad"), strip * arcPerStripRad);
      glUniform1f(primitiveBShader.getUniformLocation("halfTanStripVFov"),
          (float) Math.tan(Math.toRadians(STRIP_VERTICAL_FOV_DEG) / 2.0));
      drawFullQuad(primitiveBShader);
    }
  }

  private void drawPtzTile(DewarpConfig.PtzTile tile, int tileWidthPx, int tileHeightPx) {
    if (config.displayMode.usesPanoramaPtzTiles()) {
      drawPanoramaCrop(tile.panDeg, tile.fovDeg);
      return;
    }
    if (primitiveCShader == null) {
      primitiveCShader = new GlShader(COMPOSITE_VERTEX_SHADER, PRIMITIVE_C_FRAGMENT_SHADER);
    }
    primitiveCShader.useProgram();
    setFisheyeSampleUniforms(primitiveCShader);
    float fovHRad = (float) Math.toRadians(tile.fovDeg);
    float aspect = tileHeightPx > 0 ? tileWidthPx / (float) tileHeightPx : 1f;
    float fovVRad = 2f * (float) Math.atan(Math.tan(fovHRad / 2.0) / aspect);
    glUniform1f(primitiveCShader.getUniformLocation("panRad"), (float) Math.toRadians(tile.panDeg));
    glUniform1f(primitiveCShader.getUniformLocation("tiltRad"), (float) Math.toRadians(tile.tiltDeg));
    glUniform1f(primitiveCShader.getUniformLocation("halfTanFovH"), (float) Math.tan(fovHRad / 2.0));
    glUniform1f(primitiveCShader.getUniformLocation("halfTanFovV"), (float) Math.tan(fovVRad / 2.0));
    drawFullQuad(primitiveCShader);
  }

  /**
   * Draws a horizontally-scrollable crop of the same cylindrical panorama
   * projection used by {@link #drawBaseTile}'s ordinary (non-pannable)
   * path, instead of a fixed full-arc flatten or an independent
   * rectilinear virtual-PTZ camera: {@code fovDeg} degrees of azimuth
   * centered on {@code panDeg}, at the fixed {@link
   * #STRIP_VERTICAL_FOV_DEG} vertical FOV. No tilt parameter -- both the
   * base tile and PTZ tile callers of this only pan, matching the "scrub
   * left/right through the overview, no up/down" product requirement.
   * {@code panDeg} is expected to be mutated live (see {@link
   * DewarpConfig#basePanDeg} / {@link DewarpConfig.PtzTile#panDeg}) as the
   * user drags, so this re-reads whatever the caller passes fresh every
   * frame rather than caching anything.
   */
  private void drawPanoramaCrop(float panDeg, float fovDeg) {
    if (primitiveBShader == null) {
      primitiveBShader = new GlShader(COMPOSITE_VERTEX_SHADER, PRIMITIVE_B_FRAGMENT_SHADER);
    }
    primitiveBShader.useProgram();
    setFisheyeSampleUniforms(primitiveBShader);
    float arcRad = (float) Math.toRadians(fovDeg);
    float stripStartRad = (float) Math.toRadians(panDeg) - arcRad / 2f;
    glUniform1f(primitiveBShader.getUniformLocation("arcPerStripRad"), arcRad);
    glUniform1f(primitiveBShader.getUniformLocation("stripStartRad"), stripStartRad);
    glUniform1f(primitiveBShader.getUniformLocation("halfTanStripVFov"),
        (float) Math.tan(Math.toRadians(STRIP_VERTICAL_FOV_DEG) / 2.0));
    drawFullQuad(primitiveBShader);
  }

  /**
   * Draws Primitive D (see its doc): a plain rectangular crop of the raw
   * circle, offset by ({@code panOffsetDeg}, {@code tiltOffsetDeg}) from
   * center and sized by {@code fovDeg}. These three float "degrees"
   * parameters are reused from the same {@link DewarpConfig.PtzTile}-shaped
   * inputs as {@link #drawPanoramaCrop} purely so the Dart-side model
   * doesn't need a separate shape for this mode -- there is no actual
   * angle math here, they're just linearly rescaled to normalized crop
   * units: {@code offsetNorm = offsetDeg / 90}, {@code
   * cropHalfSizeNorm = fovDeg / 180} (fovDeg=180 shows the full diameter
   * centered on the offset point; smaller values zoom in).
   */
  private void drawDirectCircleCrop(float panOffsetDeg, float tiltOffsetDeg, float fovDeg) {
    if (primitiveDShader == null) {
      primitiveDShader = new GlShader(COMPOSITE_VERTEX_SHADER, PRIMITIVE_D_FRAGMENT_SHADER);
    }
    primitiveDShader.useProgram();
    glUniform1i(primitiveDShader.getUniformLocation("sourceTex"), 0);
    glUniform2f(primitiveDShader.getUniformLocation("center"), config.centerXNorm,
        config.centerYNorm);
    glUniform1f(primitiveDShader.getUniformLocation("radius"), config.radiusNorm);
    glUniform1f(primitiveDShader.getUniformLocation("verticalFlipSign"),
        config.mountType == DewarpConfig.MountType.DESKTOP ? -1f : 1f);
    glUniform2f(primitiveDShader.getUniformLocation("cropOffsetNorm"), panOffsetDeg / 90f,
        tiltOffsetDeg / 90f);
    glUniform1f(primitiveDShader.getUniformLocation("cropHalfSizeNorm"), fovDeg / 180f);
    drawFullQuad(primitiveDShader);
  }

  private void setFisheyeSampleUniforms(GlShader shader) {
    glUniform1i(shader.getUniformLocation("sourceTex"), 0);
    glUniform2f(shader.getUniformLocation("center"), config.centerXNorm, config.centerYNorm);
    glUniform1f(shader.getUniformLocation("radius"), config.radiusNorm);
    glUniform1f(shader.getUniformLocation("rotationOffsetRad"),
        (float) Math.toRadians(config.rotationDeg));
    glUniform1f(shader.getUniformLocation("verticalFlipSign"),
        config.mountType == DewarpConfig.MountType.DESKTOP ? -1f : 1f);
  }

  private static void drawFullQuad(GlShader shader) {
    shader.setVertexAttribArray(VERTEX_ATTRIB_POSITION, 2, FULL_RECTANGLE_BUFFER);
    shader.setVertexAttribArray(VERTEX_ATTRIB_TEXCOORD, 2, FULL_RECTANGLE_TEXCOORD_BUFFER);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

  /**
   * Returns {x, y, w, h} in [0,1], origin top-left / y-down, for the given
   * tile index (0 = base tile, 1..N = PTZ tiles). See
   * {@link DewarpConfig.DisplayMode#baseTileHeightFraction()} doc:
   * proportions are a placeholder.
   */
  private static float[] tileRectNormalizedTopLeft(
      int tileIndex, int ptzTileCount, float baseTileHeightFraction) {
    if (ptzTileCount == 0) {
      return new float[] {0f, 0f, 1f, 1f};
    }
    if (tileIndex == 0) {
      return new float[] {0f, 0f, 1f, baseTileHeightFraction};
    }
    int ptzIndex = tileIndex - 1;
    int cols = (int) Math.ceil(Math.sqrt(ptzTileCount));
    int rows = (int) Math.ceil(ptzTileCount / (double) cols);
    int col = ptzIndex % cols;
    int row = ptzIndex / cols;
    float cellW = 1f / cols;
    float cellH = (1f - baseTileHeightFraction) / rows;
    return new float[] {col * cellW, baseTileHeightFraction + row * cellH, cellW, cellH};
  }

  /**
   * The on-screen canvas size this config wants for a decoded frame of
   * ({@code decodedWidth}, {@code decodedHeight}) — used to size the
   * destination Surface/texture so the tile grid above isn't stretched.
   * Placeholder formula pending pixel-accurate UI mockups; see class doc.
   */
  public static int[] compositeSize(DewarpConfig config, int decodedWidth, int decodedHeight) {
    int ptzTileCount = config.displayMode.ptzTileCount;
    if (ptzTileCount == 0) {
      if (config.displayMode.usesPanoramaBase()) {
        // A panorama strip of arcSpan degrees wide by 90 degrees tall.
        float aspect = config.displayMode.panoramaArcSpanDeg / 90f
            / config.displayMode.panoramaSplitCount;
        return new int[] {decodedWidth, Math.max(1, (int) (decodedWidth / aspect))};
      }
      return new int[] {decodedWidth, decodedWidth}; // raw circle: square canvas
    }
    // base tile (baseTileHeightFraction tall) + a roughly-square PTZ grid
    // below it, both spanning the full width.
    float baseTileHeightFraction = config.displayMode.baseTileHeightFraction();
    return new int[] {decodedWidth,
        Math.max(1, (int) (decodedWidth / (2f - baseTileHeightFraction)))};
  }
}
