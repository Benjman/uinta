#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;

out vec2 pass_uv;

uniform mat4 mvp;

void main() {
  pass_uv = in_uv;
  gl_Position = mvp * vec4(in_pos, 1.0);
}
