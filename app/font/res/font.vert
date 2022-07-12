#version 330 core
layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec3 in_color;
out vec2 pass_uv;
out vec3 pass_color;
void main() {
  pass_uv = in_uv;
  pass_color = in_color;
  gl_Position = vec4(in_pos, 0.0, 1.0);
};
