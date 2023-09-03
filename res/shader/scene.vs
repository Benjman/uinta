#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_color;

uniform mat4 u_model = mat4(1);
uniform mat4 u_view = mat4(1);
uniform mat4 u_proj = mat4(1);
uniform float u_time = 0;

flat out vec3 pass_normal;
out vec3 pass_color;
out vec4 pass_world_pos;

void main() {
  pass_color = in_color;
  pass_normal = mat3(transpose(inverse(u_model))) * in_normal;
  pass_world_pos = u_model * vec4(in_pos, 1);
  gl_Position = u_proj * u_view * pass_world_pos;
}
