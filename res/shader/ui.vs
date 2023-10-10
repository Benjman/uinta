#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;

uniform mat4 u_transform = mat4(1);
uniform mat4 u_projection = mat4(1);
uniform float u_alpha = 1;

out vec4 pass_color;

void main() {
  gl_Position = u_projection * u_transform * vec4(in_pos, 1.0);
  pass_color = vec4(in_color, u_alpha);
}
