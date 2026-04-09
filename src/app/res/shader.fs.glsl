#version 330 core
out vec4 oColor;

in vec2 pUv;

uniform sampler2D texture1;

void main() {
  oColor = texture(texture1, pUv);
}
