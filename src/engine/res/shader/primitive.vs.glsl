#version 330 core
layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNorm;
layout(location = 2) in vec4 iColor;
layout(location = 3) in vec2 iUv;

out vec3 pColor;

uniform mat4 uModel = mat4(1);
uniform mat4 uView = mat4(1);
uniform mat4 uProjection = mat4(1);

void main() {
  pColor = iColor;
  gl_Position = uProjection * uView * uModel * vec4(iPos, 1.0);
}
