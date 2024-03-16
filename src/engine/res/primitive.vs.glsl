#version 330 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNorm;
layout (location = 2) in vec3 iColor;
layout (location = 3) in vec2 iUv;

out vec3 pColor;
out vec3 pNormal;
out vec3 pPos;
out vec2 pUv;

uniform int uFlags;
uniform float uTime;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

const float swayHeight = 0.5;

// NOTE: These values need to match the values defined in `shaders/primitive.h`
const int SwayMask = 1 << 0;
const int UvMask = 1 << 1;

void main() {
  pPos = vec3(uModel * vec4(iPos, 1.0));
  pNormal = normalize(mat3(transpose(inverse(uModel))) * iNorm);
  pColor = iColor;
  pUv = iUv;

  if ((uFlags & SwayMask) != 0) {
    float heightAbove = max(pPos.y - swayHeight, 0.0);
    float swayFactor = sin(uTime + pPos.x) * heightAbove * 0.05;
    pPos = vec3(pPos.x + swayFactor, pPos.y + swayFactor * 0.1, pPos.z + swayFactor);
  }

  gl_Position = uProjection * uView * vec4(pPos, 1.0);
}
