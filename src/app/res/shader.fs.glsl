#version 330 core
out vec4 out_color;

in vec2 pass_texCoord;

uniform sampler2D texture1;

void main() {
  out_color = texture(texture1, pass_texCoord);
}
