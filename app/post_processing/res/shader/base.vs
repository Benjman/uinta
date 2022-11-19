#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_color;

out vec4 pass_color;

uniform mat4 mvp;

void main() {
  pass_color = vec4(in_color, 1);
  gl_Position = mvp * vec4(in_pos, 1);
}
