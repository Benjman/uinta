#version 330 core

in vec3 vWorldPos;

out vec4 oColor;

uniform vec4 uColor = vec4(1);
uniform vec3 uCameraPos;
uniform float uFadeStart = 20.0;
uniform float uFadeEnd = 30.0;
uniform int uFadeEnabled = 1;

void main() {
  float alpha = 1.0;
  if (uFadeEnabled != 0) {
    float dist = distance(vWorldPos, uCameraPos);
    alpha = 1.0 - smoothstep(uFadeStart, uFadeEnd, dist);
  }
  oColor = vec4(uColor.rgb, uColor.a * alpha);
}
