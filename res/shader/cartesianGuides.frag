#version 330 core

in vec3 pass_color;

uniform vec3 u_color;
uniform vec2 u_viewport;
uniform float u_bezierStrength;

out vec4 out_color;

float cubicBezier(float t) {
  return u_bezierStrength + (--t) * t * t;
}

void main() {
  vec3 coord = vec3(gl_FragCoord) / vec3(u_viewport, 1.0);
  out_color = vec4(u_color, cubicBezier(0.5 - length(vec3(0.5) - coord)));
}
