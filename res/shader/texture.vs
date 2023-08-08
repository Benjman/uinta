#version 330 core
layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec2 in_uv;

out vec2 pass_uv;

void main() {
  gl_Position = vec4(vec3(in_pos, 0.0), 1.0);
  pass_uv = in_uv;
}
