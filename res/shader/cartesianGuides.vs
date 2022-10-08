#version 330 core

layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec3 in_color;

out vec4 pass_color;

uniform mat4 u_mvp;

void main() {
  pass_color = vec4(in_color, 1.0);
  gl_Position = u_mvp * vec4(in_pos.x, 0.0, in_pos.y, 1.0);
}
