#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;

out vec3 pass_color;

uniform mat4 view;
uniform mat4 projection;

void main() {
  pass_color = normalize(in_color);
  gl_Position = projection * view * vec4(in_pos, 1.0);
}
