#version 330 core
layout (location = 0) in vec2 iPos;

uniform mat4 uProjection = mat4(1);

void main() {
  gl_Position = uProjection * vec4(iPos, -1.0, 0.5);
}
