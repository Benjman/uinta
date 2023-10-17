#version 330 core

in vec2 pass_uv;
in vec3 pass_color;

uniform sampler2D u_atlas;
uniform float u_alpha = 1;

out vec4 out_color;

void main() {
  out_color = vec4(pass_color, texture(u_atlas, pass_uv).r);
};
