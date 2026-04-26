#version 330 core

// Tonemapping pass. Maps HDR scene-referred linear color to an LDR
// display-referred sRGB signal. Three operators are offered:
//
//   0 : AgX (Troy Sobotka fitted approximation). Current default in Godot 4
//       / Blender; handles saturated stylized colors more gracefully than
//       filmic ACES.
//   1 : ACES fitted (Stephen Hill). Classic "cinematic" response curve.
//   2 : Reinhard-Jodie. Cheapest, neutral look.
//
// The `uExposure` multiplier is applied in linear space before tonemapping.
// sRGB gamma encoding is applied as the final step.

in vec2 pUv;
out vec4 oColor;

uniform sampler2D uInput;
uniform float uExposure;
uniform int uOperator;

// ---------- AgX (Troy Sobotka, 2023; fitted approximation) ----------
//
// Based on Troy Sobotka's AgX display-rendering transform. This is the
// "minimal" fit published in the AgX repository / Blender sources, good
// enough for real-time use. Operates on linear-scene input and returns
// linear-display output (sRGB primaries).

const mat3 AgXInsetMatrix = mat3(
  0.842479062253094, 0.0423282422610123, 0.0423756549057051,
  0.0784335999999992, 0.878468636469772, 0.0784336,
  0.0792237451477643, 0.0791661274605434, 0.879142973793104);

const mat3 AgXOutsetMatrix = mat3(
   1.19687900512017,  -0.0528968517574562, -0.0529716355144438,
  -0.0980208811401368, 1.15190312990417,   -0.0980434501171241,
  -0.0990297440797205,-0.0989611768448433,  1.15107367264116);

vec3 agxDefaultContrastApprox(vec3 x) {
  vec3 x2 = x * x;
  vec3 x4 = x2 * x2;
  return +15.5     * x4 * x2
         - 40.14    * x4 * x
         + 31.96    * x4
         - 6.868    * x2 * x
         + 0.4298   * x2
         + 0.1191   * x
         - 0.00232;
}

vec3 agx(vec3 c) {
  const float minEv = -12.47393;
  const float maxEv =  4.026069;
  c = AgXInsetMatrix * c;
  c = max(c, vec3(1e-10));
  c = log2(c);
  c = (c - minEv) / (maxEv - minEv);
  c = clamp(c, 0.0, 1.0);
  c = agxDefaultContrastApprox(c);
  c = AgXOutsetMatrix * c;
  // AgX outputs in linear; caller applies the final sRGB OETF.
  return c;
}

// ---------- ACES fitted (Stephen Hill) ----------
const mat3 ACESInputMat = mat3(
  0.59719, 0.07600, 0.02840,
  0.35458, 0.90834, 0.13383,
  0.04823, 0.01566, 0.83777);

const mat3 ACESOutputMat = mat3(
   1.60475, -0.10208, -0.00327,
  -0.53108,  1.10813, -0.07276,
  -0.07367, -0.00605,  1.07602);

vec3 rrtAndOdtFit(vec3 v) {
  vec3 a = v * (v + 0.0245786) - 0.000090537;
  vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
  return a / b;
}

vec3 aces(vec3 c) {
  c = ACESInputMat * c;
  c = rrtAndOdtFit(c);
  c = ACESOutputMat * c;
  return clamp(c, 0.0, 1.0);
}

// ---------- Reinhard-Jodie ----------
vec3 reinhard(vec3 c) {
  float l = dot(c, vec3(0.2126, 0.7152, 0.0722));
  vec3 tc = c / (c + vec3(1.0));
  return mix(c / (l + 1.0), tc, tc);
}

// ---------- sRGB OETF ----------
vec3 linearToSrgb(vec3 c) {
  c = max(c, vec3(0.0));
  bvec3 hi = greaterThan(c, vec3(0.0031308));
  vec3 a = 12.92 * c;
  vec3 b = 1.055 * pow(c, vec3(1.0 / 2.4)) - 0.055;
  return mix(a, b, vec3(hi));
}

void main() {
  vec3 hdr = texture(uInput, pUv).rgb * uExposure;

  vec3 ldr;
  if (uOperator == 1) {
    ldr = aces(hdr);
  } else if (uOperator == 2) {
    ldr = reinhard(hdr);
  } else {
    ldr = agx(hdr);
  }

  oColor = vec4(linearToSrgb(ldr), 1.0);
}
