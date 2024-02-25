#version 330 core

layout (location = 0) in vec2 iPos;

uniform mat4 uModelView;
uniform mat4 uProjection;

#define epsilon 0.001

void main() {
  gl_Position = uProjection * uModelView * vec4(iPos, epsilon, 1.0).xzyw;
}
