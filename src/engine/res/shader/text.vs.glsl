#version 330 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec4 iColor;
layout (location = 2) in vec2 iUv;

uniform mat4 uTransform;
uniform mat4 uProjection;

out vec2 pUv;
out vec4 pColor;

void main() {
  gl_Position = uProjection * uTransform * vec4(iPos, 1.0);
  pUv = iUv;
  pColor = iColor;
}
