#version 330 core

// 13-tap weighted downsample for bloom, after Jimenez 2014 (Call of Duty:
// Advanced Warfare). Samples a 4x4 neighborhood from the source mip and
// combines them in 5 overlapping 2x2 groups with boosted center weighting.
//
// On the very first downsample (uIsFirst != 0), each group is averaged
// under a partial Karis operator to crush fireflies (HDR pixels above the
// renderer's ~1.0 threshold that would otherwise alias in the mip chain).

in vec2 pUv;
out vec4 oColor;

uniform sampler2D uInput;
uniform vec2 uTexelSize;  // 1.0 / size of uInput
uniform int  uIsFirst;    // non-zero on the first mip-0 downsample

float karisAverage(vec3 c) {
  // Luma-weighted inverse averaging used in Jimenez's talk.
  float luma = dot(c, vec3(0.2126, 0.7152, 0.0722)) * 0.25;
  return 1.0 / (1.0 + luma);
}

void main() {
  float x = uTexelSize.x;
  float y = uTexelSize.y;

  vec3 a = texture(uInput, pUv + vec2(-2.0 * x,  2.0 * y)).rgb;
  vec3 b = texture(uInput, pUv + vec2( 0.0,      2.0 * y)).rgb;
  vec3 c = texture(uInput, pUv + vec2( 2.0 * x,  2.0 * y)).rgb;

  vec3 d = texture(uInput, pUv + vec2(-2.0 * x,  0.0)).rgb;
  vec3 e = texture(uInput, pUv + vec2( 0.0,      0.0)).rgb;
  vec3 f = texture(uInput, pUv + vec2( 2.0 * x,  0.0)).rgb;

  vec3 g = texture(uInput, pUv + vec2(-2.0 * x, -2.0 * y)).rgb;
  vec3 h = texture(uInput, pUv + vec2( 0.0,     -2.0 * y)).rgb;
  vec3 i = texture(uInput, pUv + vec2( 2.0 * x, -2.0 * y)).rgb;

  vec3 j = texture(uInput, pUv + vec2(-x,  y)).rgb;
  vec3 k = texture(uInput, pUv + vec2( x,  y)).rgb;
  vec3 l = texture(uInput, pUv + vec2(-x, -y)).rgb;
  vec3 m = texture(uInput, pUv + vec2( x, -y)).rgb;

  vec3 result;
  if (uIsFirst != 0) {
    // Partial Karis average per group to kill fireflies.
    vec3 g0 = (a + b + d + e) * 0.25;
    vec3 g1 = (b + c + e + f) * 0.25;
    vec3 g2 = (d + e + g + h) * 0.25;
    vec3 g3 = (e + f + h + i) * 0.25;
    vec3 g4 = (j + k + l + m) * 0.25;

    float w0 = karisAverage(g0);
    float w1 = karisAverage(g1);
    float w2 = karisAverage(g2);
    float w3 = karisAverage(g3);
    float w4 = karisAverage(g4);
    float wSum = w0 + w1 + w2 + w3 + w4 + 1e-5;

    // Jimenez weights: outer groups 0.125 each, center group 0.5.
    result = (g0 * 0.125 * w0 +
              g1 * 0.125 * w1 +
              g2 * 0.125 * w2 +
              g3 * 0.125 * w3 +
              g4 * 0.5   * w4) / wSum * (0.125 * 4.0 + 0.5);
  } else {
    // Standard weighting.
    result  = e * 0.125;
    result += (a + c + g + i) * 0.03125;
    result += (b + d + f + h) * 0.0625;
    result += (j + k + l + m) * 0.125;
  }

  oColor = vec4(max(result, vec3(0.0)), 1.0);
}
