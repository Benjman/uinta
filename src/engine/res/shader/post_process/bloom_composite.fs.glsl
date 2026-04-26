#version 330 core

// Bloom composite: blends the bloom mip-chain top (uBloom) into the base
// scene color (uInput) using a linear mix. The bloom image is itself HDR;
// the tonemap pass downstream is responsible for mapping the summed result
// to display.

in vec2 pUv;
out vec4 oColor;

uniform sampler2D uInput;
uniform sampler2D uBloom;
uniform float uIntensity;  // 0 disables bloom, 1 is full bloom
uniform float uThreshold;  // soft-knee brightness threshold applied to bloom

// Soft-knee prefilter (Karis / Unity Bloom). Below `uThreshold` the bloom
// contribution is fully suppressed; from `uThreshold` to `uThreshold + 0.5`
// it ramps in quadratically; above that it passes through. When
// `uThreshold <= 0` the input is returned unchanged.
//
// Note: the canonical place to apply this knee is in the *first downsample*
// (so the rest of the bloom mip chain only carries above-threshold energy).
// We apply it at composite as a cheap approximation that still attenuates
// faint chain residue.
vec3 prefilter(vec3 c) {
  if (uThreshold <= 0.0) {
    return c;
  }
  float brightness = max(c.r, max(c.g, c.b));
  float soft = brightness - uThreshold;
  soft = clamp(soft, 0.0, 0.5);
  soft = soft * soft * 2.0;
  float contribution = max(soft, brightness - uThreshold);
  contribution /= max(brightness, 1e-5);
  return c * contribution;
}

void main() {
  vec3 scene = texture(uInput, pUv).rgb;
  vec3 bloom = prefilter(texture(uBloom, pUv).rgb);
  // Scene passes through unchanged; bloom is mixed in additively (scaled).
  vec3 result = scene + bloom * uIntensity;
  oColor = vec4(result, 1.0);
}
