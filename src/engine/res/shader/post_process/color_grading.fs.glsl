#version 330 core

// 3D LUT color grading using a 2D LUT-strip encoding.
//
// The LUT is a `(N*N) x N` 2D texture, where each horizontal strip of size
// N x N represents one slice of the 3D cube along the blue axis. Blue index
// 0 is leftmost; blue index N-1 is rightmost. This is the same format
// exported by "CubeLUT to PNG" tools and accepted by Unity / Unreal.
//
// `uLutSize` is N (the cube edge length); 16 or 32 are common. Trilinear
// filtering across blue slices is done manually.

in vec2 pUv;
out vec4 oColor;

uniform sampler2D uInput;
uniform sampler2D uLut;
uniform float uLutSize;       // edge length N; 0 disables grading.
uniform float uIntensity;     // 0 = no grading, 1 = full grading.

vec3 sampleLut(vec3 c) {
  vec3 clamped = clamp(c, vec3(0.0), vec3(1.0));
  float n = uLutSize;
  float blue = clamped.b * (n - 1.0);
  float bLo = floor(blue);
  float bHi = min(bLo + 1.0, n - 1.0);
  float bT  = blue - bLo;

  // Pixel size in strip space.
  float pixelW = 1.0 / (n * n);
  float pixelH = 1.0 / n;
  // Center samples within their strip cells.
  float u = (clamped.r * (n - 1.0) + 0.5) * pixelW;
  float v = (clamped.g * (n - 1.0) + 0.5) * pixelH;

  vec3 loSlice = texture(uLut, vec2(u + bLo / n, v)).rgb;
  vec3 hiSlice = texture(uLut, vec2(u + bHi / n, v)).rgb;
  return mix(loSlice, hiSlice, bT);
}

void main() {
  vec3 src = texture(uInput, pUv).rgb;
  if (uLutSize <= 0.0 || uIntensity <= 0.0) {
    oColor = vec4(src, 1.0);
    return;
  }
  vec3 graded = sampleLut(src);
  oColor = vec4(mix(src, graded, clamp(uIntensity, 0.0, 1.0)), 1.0);
}
