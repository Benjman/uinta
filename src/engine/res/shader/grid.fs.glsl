#version 330 core

in vec3 vWorldPos;

out vec4 oColor;

uniform vec4 uColor = vec4(1);
uniform vec3 uCameraPos;

void main() {
  float alpha = 1.0;
  oColor = vec4(uColor.rgb, uColor.a * alpha);
}
