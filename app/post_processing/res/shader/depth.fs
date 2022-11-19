#version 330 core

out vec4 out_color;

uniform float u_nearPlane;
uniform float u_farPlane;

float linearizeDepth(float depth) {
  float z = depth * 2.0 - 1.0; // back to ndc
  return (2.0 * u_nearPlane * u_farPlane) / (u_farPlane + u_nearPlane - z * (u_farPlane - u_nearPlane));	
}

void main() {
  float depth = linearizeDepth(gl_FragCoord.z) / u_farPlane; // divide by u_farPlane for demonstration
  out_color = vec4(vec3(depth), 1.0);
}
