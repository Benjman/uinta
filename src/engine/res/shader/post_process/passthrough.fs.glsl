#version 330 core

in vec2 pUv;
out vec4 oColor;

uniform sampler2D uInput;

void main() {
  oColor = vec4(texture(uInput, pUv).rgb, 1.0);
}
