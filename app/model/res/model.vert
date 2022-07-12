#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_norm;

uniform mat4 u_model = mat4(1.0);

out vec3 pass_norm;
out vec3 pass_frag_pos;

void main() {
  pass_norm = normalize(mat3(transpose(inverse(u_model))) * normalize(in_norm));
  pass_frag_pos = vec3(u_model * vec4(in_pos, 1.0));
  gl_Position = u_model * vec4(in_pos, 1.0);
}
