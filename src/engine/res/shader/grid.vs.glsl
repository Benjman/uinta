#version 330 core

layout (location = 0) in vec2 iPos;

out vec3 vWorldPos;

uniform mat4 uModelView;
uniform mat4 uProjection;
uniform vec2 uGridOffset = vec2(0.0);

#define epsilon 0.001

void main() {
  // Apply grid offset to follow camera (snapped to cell boundaries)
  vec2 offsetPos = iPos + uGridOffset;
  // World position: offsetPos.x -> x, epsilon -> y, offsetPos.y -> z
  vWorldPos = vec3(offsetPos.x, epsilon, offsetPos.y);
  gl_Position = uProjection * uModelView * vec4(offsetPos, epsilon, 1.0).xzyw;
}
