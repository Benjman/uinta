#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;

out vec3 pColor;

uniform mat4 uView;
uniform mat4 uProjection;

void main() {
  pColor = normalize(in_color);
  gl_Position = uProjection * uView * vec4(in_pos, 1.0);
}
