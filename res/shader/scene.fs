#version 330 core

flat in vec3 pass_normal;
in vec3 pass_color;

out vec4 out_color;

uniform vec3 u_lightDir;
uniform vec3 u_lightColor = vec3(1);

void main() {
  // ambient
  float ambientStrength = 0.25;
  vec3 ambient = ambientStrength * u_lightColor;

  //diffuse
  vec3 norm = normalize(pass_normal);
  float diff = max(dot(norm, u_lightDir), 0);
  vec3 diffuse = diff * u_lightColor;

  out_color = vec4((ambient + diffuse) * pass_color, 1);
}
