#version 330 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNorm;
layout (location = 2) in vec3 iColor;
layout (location = 3) in vec2 iUv;

out vec2 pUv;

uniform mat4 uProjection = mat4(1);

void main() {
  gl_Position = uProjection * vec4(iPos, 0.5);
  pUv = vec2(iUv.x, iUv.y);
}
