#version 330 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNorm;
layout (location = 2) in vec3 iColor;
layout (location = 3) in vec2 iUv;

out vec3 pColor;
out vec3 pNormal;
out vec3 pPos;
out vec2 pUv;

uniform int uFlags = 0;
uniform float uTime = 0;
uniform mat4 uModel = mat4(1);
uniform mat4 uView = mat4(1);
uniform mat4 uProjection = mat4(1);

const float swayHeight = 0.5;

bool isSwayEnabled(int flags) {
  const int SwayMask = 1 << 0;
  return (flags & SwayMask) != 0;
}

bool isUvEnabled(int flags) {
  const int UvMask = 1 << 1;
  return (flags & UvMask) != 0;
}

bool isNormalsEnabled(int flags) {
  const int NormalMask = 1 << 2;
  return (flags & NormalMask) != 0;
}

void main() {
  pPos = vec3(uModel * vec4(iPos, 1.0));
  pNormal = normalize(mat3(transpose(inverse(uModel))) * iNorm);
  pColor = iColor;
  pUv = iUv;

  if (isSwayEnabled(uFlags)) {
    float heightAbove = max(pPos.y - swayHeight, 0.0);
    float swayFactor = sin(uTime + pPos.x) * heightAbove * 0.05;
    pPos = vec3(pPos.x + swayFactor, pPos.y + swayFactor * 0.1, pPos.z + swayFactor);
  }

  gl_Position = uProjection * uView * vec4(pPos, 1.0);
}
