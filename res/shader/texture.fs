#version 330 core
out vec4 out_color;

in vec2 pass_uv;

// texture sampler
uniform sampler2D texture1;

void main() {
  out_color = vec4(vec3(texture(texture1, pass_uv).r), 1);
}
