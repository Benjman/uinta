#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_norm;
layout (location = 2) in vec3 in_color;

uniform mat4 u_model = mat4(1);
uniform mat4 u_mvp = mat4(1);

out vec3 pass_norm;
out vec3 pass_frag_pos;
out vec3 pass_color;

void main() {
  pass_color = in_color;
  pass_norm = normalize(mat3(transpose(inverse(u_model))) * normalize(in_norm));
  pass_frag_pos = vec3(u_model * vec4(in_pos, 1));
  gl_Position = u_mvp * vec4(in_pos, 1);
}
