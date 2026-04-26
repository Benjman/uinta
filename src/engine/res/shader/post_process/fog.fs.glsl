#version 330 core

// Depth-based exponential fog. Reconstructs world-space position from the
// scene's depth buffer via the inverse view-projection matrix and computes
// Euclidean distance from the camera in world units.
//
//   fog = 1 - exp(-distance * density)
//
// As depth approaches the far clipping plane, the fog factor is smoothly
// driven to 1.0 over the last `1 - uFarFadeStart` fraction of the
// linearized view distance (i.e. from `uFarFadeStart * uFar` out to `uFar`).
// This avoids a hard discontinuity at the sky / far plane and lets sky
// pixels converge to `uFogColor` as part of the same fade everything else
// uses. Set `uFarFadeStart >= 1.0` to disable the fade.

in vec2 pUv;
out vec4 oColor;

uniform sampler2D uInput;
uniform sampler2D uDepth;

uniform mat4 uInvViewProjection;
uniform vec3 uCameraPos;
uniform vec3 uFogColor;
uniform float uDensity;
uniform float uNear;
uniform float uFar;
uniform float uFarFadeStart;  // fraction of uFar at which the fade begins

// Convert a non-linear depth-buffer value [0,1] into a linear view-space
// distance along the camera Z axis, in the same units as uNear / uFar.
float linearDepth(float d) {
  float z = d * 2.0 - 1.0;
  return (2.0 * uNear * uFar) / (uFar + uNear - z * (uFar - uNear));
}

vec3 worldPosFromDepth(vec2 uv, float depth) {
  vec4 clip = vec4(uv * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
  vec4 world = uInvViewProjection * clip;
  return world.xyz / world.w;
}

void main() {
  vec3 src = texture(uInput, pUv).rgb;
  float d = texture(uDepth, pUv).r;

  vec3 world = worldPosFromDepth(pUv, d);
  float dist = length(world - uCameraPos);

  float fog = clamp(1.0 - exp(-dist * uDensity), 0.0, 1.0);

  // Far-plane fade: drive the fog factor to 1.0 over the configured
  // fade window in linearized view distance. This replaces the hard
  // sky-punch that used to live here as a magic-constant comparison on
  // raw NDC depth. `uFarFadeStart >= 1.0` disables the fade.
  float linDist = linearDepth(d);
  float fadeStart = clamp(uFarFadeStart, 0.0, 0.9999) * uFar;
  float farFade = smoothstep(fadeStart, uFar, linDist);
  fog = mix(fog, 1.0, farFade);

  oColor = vec4(mix(src, uFogColor, fog), 1.0);
}
