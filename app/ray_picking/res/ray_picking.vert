#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;

out vec3 pass_color;

uniform mat4 u_mvp = mat4(1.0);

void main() {
  pass_color = in_color;
  gl_Position = u_mvp * vec4(in_pos, 1.0);
}
