#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec3 in_color;

uniform mat4 u_transform = mat4(1);
uniform mat4 u_projection = mat4(1);

out vec2 pass_uv;
out vec3 pass_color;

void main() {
  gl_Position = u_projection * u_transform * vec4(in_pos, 1.0);
  pass_uv = in_uv;
  pass_color = in_color;
};
