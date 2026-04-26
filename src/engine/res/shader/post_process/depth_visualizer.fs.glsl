#version 330 core

in vec2 pUv;
out vec4 oColor;

uniform sampler2D uDepth;
uniform float uNear;
uniform float uFar;

float linearizeDepth(float depth) {
  // depth is in [0,1] representing window-space. Convert to NDC then to
  // eye-space linear depth using the standard perspective-projection inverse.
  float z = depth * 2.0 - 1.0;  // NDC
  return (2.0 * uNear * uFar) / (uFar + uNear - z * (uFar - uNear));
}

void main() {
  float d = texture(uDepth, pUv).r;
  float linear = linearizeDepth(d) / uFar;  // normalize to [0,1]
  oColor = vec4(vec3(linear), 1.0);
}
