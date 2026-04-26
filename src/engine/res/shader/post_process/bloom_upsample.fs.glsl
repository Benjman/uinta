#version 330 core

// 9-tap 3x3 tent upsample filter (Jimenez 2014). Samples the smaller mip
// and "spreads" it into the larger mip using a separable tent.
//
// The caller is expected to additively blend the result into the destination
// mip using glBlendFunc(GL_ONE, GL_ONE) so each upsample contributes to the
// growing bloom image.

in vec2 pUv;
out vec4 oColor;

uniform sampler2D uInput;
uniform float uFilterRadius;  // in texels; ~0.005-0.01 works well

void main() {
  float x = uFilterRadius;
  float y = uFilterRadius;

  vec3 a = texture(uInput, pUv + vec2(-x,  y)).rgb;
  vec3 b = texture(uInput, pUv + vec2( 0.0,  y)).rgb;
  vec3 c = texture(uInput, pUv + vec2( x,  y)).rgb;

  vec3 d = texture(uInput, pUv + vec2(-x, 0.0)).rgb;
  vec3 e = texture(uInput, pUv + vec2( 0.0, 0.0)).rgb;
  vec3 f = texture(uInput, pUv + vec2( x, 0.0)).rgb;

  vec3 g = texture(uInput, pUv + vec2(-x, -y)).rgb;
  vec3 h = texture(uInput, pUv + vec2( 0.0, -y)).rgb;
  vec3 i = texture(uInput, pUv + vec2( x, -y)).rgb;

  // Tent weights: center 4, edges 2, corners 1, divided by 16.
  vec3 result = e * 4.0;
  result += (b + d + f + h) * 2.0;
  result += (a + c + g + i);
  result *= 1.0 / 16.0;

  oColor = vec4(result, 1.0);
}
