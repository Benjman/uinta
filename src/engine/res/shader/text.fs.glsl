#version 330 core

in vec4 pColor;
in vec2 pUv;

uniform sampler2D uAtlas;

out vec4 oColor;

void main() {
  oColor = vec4(pColor.rgb, texture(uAtlas, pUv).r * pColor.a);
}
