#version 330 core

in vec3 pColor;
in vec2 pUv;

uniform sampler2D uAtlas;
uniform float uAlpha;

out vec4 oColor;

void main() {
  oColor = vec4(pColor, texture(uAtlas, pUv).r * uAlpha);
}
