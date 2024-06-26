#version 330 core

layout (location = 0) in vec2 iPos;

uniform mat4 uModelView;
uniform mat4 uProjection;

void main() {
  gl_Position = uProjection * uModelView * vec4(iPos, 0, 1.0).xzyw;
}
