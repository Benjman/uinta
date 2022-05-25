#version 330 core

uniform mat4 u_mvp = mat4(1.0);

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_norm;
layout (location = 2) in vec3 in_color;

out vec3 pass_color;
out vec3 pass_norm;

void main() {
  pass_norm = normalize(mat3(transpose(inverse(u_mvp))) * normalize(in_norm));
  pass_color = in_color;
  gl_Position = u_mvp * vec4(in_pos, 1.0);
}
