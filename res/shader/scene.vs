#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform float u_time;

out vec3 pass_normal;
out vec3 pass_color;

void main() {
  pass_color = in_color;
  pass_normal = mat3(transpose(inverse(u_model))) * in_normal;
  gl_Position = u_proj * u_view * u_model * vec4(in_pos, 1);
}
