#version 330 core

flat in vec3 pass_normal;
in vec3 pass_color;
in vec4 pass_world_pos;

out vec4 out_color;

uniform vec3 u_lightDir = vec3(0, -1, 0);
uniform vec3 u_lightColor = vec3(1);

// TODO: Move me to the terrain shader once implemented
float noise(vec2 st) {
    vec2 pos = floor(st);
    vec2 frac = fract(st);
    float randomValue = fract(sin(dot(pos, vec2(12.9898, 78.233))) * 43758.5453);
    return mix(randomValue, 1.0 - randomValue, frac.x * frac.y);
}

void main() {
  float n = noise(pass_world_pos.xz * 10.0);

  // ambient
  float ambientStrength = 0.25;
  vec3 ambient = ambientStrength * u_lightColor;

  // diffuse
  vec3 norm = normalize(pass_normal);
  float diff = max(dot(norm, u_lightDir), 0);
  vec3 diffuse = diff * u_lightColor;

  // Combine ambient and diffuse lighting with color, then blend with noise
  vec3 color = (ambient + diffuse) * pass_color;
  out_color = vec4(mix(color, color * n, 0.1), 1); // Blending with noise here
}
