#version 330 core
in vec2 pass_uv;
in vec3 pass_color;
uniform sampler2D atlas;
out vec4 out_color;
void main() {
  out_color = vec4(pass_color, texture(atlas, pass_uv).r);
};
