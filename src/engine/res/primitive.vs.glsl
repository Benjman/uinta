#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_norm;
layout (location = 2) in vec3 in_color;
layout (location = 3) in vec2 in_uv;

out vec3 pass_color;
out vec3 pass_normal;
out vec3 pass_pos;
out vec2 pass_uv;

uniform bool sway = false;
uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float swayHeight = 0.5;

void main() {
  pass_pos = vec3(model * vec4(in_pos, 1.0));
  pass_normal = mat3(transpose(inverse(model))) * in_norm;
  pass_color = in_color;
  pass_uv = in_uv;

  if (sway) {
    float heightAbove = max(in_pos.y - swayHeight, 0.0);
    float swayFactor = sin(time + pass_pos.y) * heightAbove * 0.05;
    pass_pos = vec3(in_pos.x + swayFactor, in_pos.y + swayFactor * 0.1, in_pos.z + swayFactor);
  }

  gl_Position = projection * view * model * vec4(pass_pos, 1.0);
}
