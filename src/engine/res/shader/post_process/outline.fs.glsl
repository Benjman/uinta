#version 330 core

// Cel/toon outline pass. Detects silhouette and crease edges by combining
// two signals:
//   1. Linearized scene depth (silhouette against background).
//   2. Reconstructed view-space normals using a "best of four" pick that
//      avoids straddling depth discontinuities.
//
// Normals are reconstructed from depth via the inverse projection matrix;
// no G-buffer is required. The two edge signals are summed and used to
// blend the outline color over the scene.
//
// As linearized depth approaches the far clipping plane, the edge signal
// is smoothly faded to zero over the last `1 - uFarFadeStart` fraction of
// the view distance (i.e. from `uFarFadeStart * uFar` out to `uFar`). This
// kills spurious silhouette outlines on the sky / horizon where view-space
// normal reconstruction degenerates and where the depth buffer's quantization
// produces deltas that even distance-normalized edge detection can't tame.
// Set `uFarFadeStart >= 1.0` to disable the fade.

in vec2 pUv;
out vec4 oColor;

uniform sampler2D uInput;
uniform sampler2D uDepth;
uniform mat4 uInvProjection;
uniform vec2 uTexelSize;
uniform vec3 uOutlineColor;
uniform float uThickness;
uniform float uDepthSensitivity;
uniform float uNormalSensitivity;
uniform float uNear;
uniform float uFar;
uniform float uFarFadeStart;  // fraction of uFar at which the fade begins

// Convert hardware depth (0..1) to view-space distance in world units.
float linearDepth(float d) {
  float z = d * 2.0 - 1.0;
  return (2.0 * uNear * uFar) / (uFar + uNear - z * (uFar - uNear));
}

// Project NDC back to view-space using the inverse projection.
vec3 viewPos(vec2 uv, float depth) {
  vec4 clip = vec4(uv * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
  vec4 view = uInvProjection * clip;
  return view.xyz / view.w;
}

// Length-guarded normalize. Returns the +Z view-space axis when the input
// is degenerate, which keeps `dot(n, n2)` at 1.0 (no false crease) on flat
// surfaces where all 5 depth samples are identical.
vec3 safeNormalize(vec3 v) {
  float l = length(v);
  return l > 1e-6 ? v / l : vec3(0.0, 0.0, 1.0);
}

void main() {
  vec3 src = texture(uInput, pUv).rgb;
  if (uThickness <= 0.0) {
    oColor = vec4(src, 1.0);
    return;
  }

  // The depth texture uses GL_NEAREST sampling, so a sub-texel offset would
  // round to the same texel as the center sample, producing zero depth
  // deltas and degenerate reconstructed normals. Clamp the geometric
  // offset to at least one texel and use the sub-1.0 thickness as a
  // strength attenuation that fades the edge in smoothly.
  vec2 offset = uTexelSize * max(uThickness, 1.0);
  float strength = clamp(uThickness, 0.0, 1.0);

  // Sample 5-tap depth neighborhood (raw NDC depth from the depth buffer).
  float dC = texture(uDepth, pUv).r;
  float dL = texture(uDepth, pUv + vec2(-offset.x, 0.0)).r;
  float dR = texture(uDepth, pUv + vec2( offset.x, 0.0)).r;
  float dU = texture(uDepth, pUv + vec2(0.0,  offset.y)).r;
  float dD = texture(uDepth, pUv + vec2(0.0, -offset.y)).r;

  // Linearize each so edge magnitudes are in world units, not in the
  // hyperbolic NDC-z space (where small NDC differences far from the camera
  // correspond to enormous world distances).
  float lC = linearDepth(dC);
  float lL = linearDepth(dL);
  float lR = linearDepth(dR);
  float lU = linearDepth(dU);
  float lD = linearDepth(dD);

  // Roberts cross on linearized depth, normalized by the center distance
  // so sensitivity is roughly view-distance-invariant. Without the 1/lC
  // term, edges would still firework at the far plane in spite of the
  // linearization (a 1m gap appears the same whether 5m or 500m away).
  float depthEdge =
      (abs(lL - lR) + abs(lU - lD)) / max(lC, 1e-4) * uDepthSensitivity;

  // --- Best-of-four normal reconstruction ---------------------------------
  // For each axis, prefer the neighbor whose linearized depth is closest to
  // the center sample. This avoids reconstructing a normal across a depth
  // discontinuity (silhouette edge), which would otherwise produce a
  // garbage normal and a false crease line that stacks on top of the
  // silhouette.
  vec3 pC = viewPos(pUv, dC);

  bool useRight = abs(lR - lC) < abs(lL - lC);
  bool useUp    = abs(lU - lC) < abs(lD - lC);

  vec3 pH = useRight ? viewPos(pUv + vec2( offset.x, 0.0), dR)
                     : viewPos(pUv + vec2(-offset.x, 0.0), dL);
  vec3 pV = useUp    ? viewPos(pUv + vec2(0.0,  offset.y), dU)
                     : viewPos(pUv + vec2(0.0, -offset.y), dD);

  // Orient the reconstructed tangent vectors so the cross product yields a
  // normal that points consistently toward the camera regardless of which
  // neighbor we picked. Without these flips, switching neighbor sides flips
  // the normal direction and produces erroneous edges.
  vec3 tH = useRight ? (pH - pC) : (pC - pH);
  vec3 tV = useUp    ? (pV - pC) : (pC - pV);

  vec3 n = safeNormalize(cross(tH, tV));

  // Compare against the alternate axis pair to detect creases. We pick the
  // *other* side for each axis and reconstruct a second normal; a high
  // angle between the two indicates a crease edge.
  vec3 pH2 = useRight ? viewPos(pUv + vec2(-offset.x, 0.0), dL)
                      : viewPos(pUv + vec2( offset.x, 0.0), dR);
  vec3 pV2 = useUp    ? viewPos(pUv + vec2(0.0, -offset.y), dD)
                      : viewPos(pUv + vec2(0.0,  offset.y), dU);
  vec3 tH2 = useRight ? (pC - pH2) : (pH2 - pC);
  vec3 tV2 = useUp    ? (pC - pV2) : (pV2 - pC);
  vec3 n2 = safeNormalize(cross(tH2, tV2));

  float normalEdge = (1.0 - max(dot(n, n2), 0.0)) * uNormalSensitivity;

  // Fade the edge in for sub-pixel thickness (uThickness in (0, 1)) so the
  // user-facing slider behaves continuously despite the per-texel snap.
  float edge = clamp(depthEdge + normalEdge, 0.0, 1.0) * strength;

  // Far-plane fade: smoothly attenuate the edge signal to zero over the
  // configured fade window. Keeps silhouettes crisp on near/mid geometry
  // while preventing the horizon/sky from being outlined.
  // `uFarFadeStart >= 1.0` disables the fade.
  float fadeStart = clamp(uFarFadeStart, 0.0, 0.9999) * uFar;
  float farFade = 1.0 - smoothstep(fadeStart, uFar, lC);
  edge *= farFade;

  oColor = vec4(mix(src, uOutlineColor, edge), 1.0);
}
