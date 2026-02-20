#version 330 core

out vec4 oColor;

uniform vec4 uColor = vec4(1);

void main() {
  oColor = vec4(uColor);
}
