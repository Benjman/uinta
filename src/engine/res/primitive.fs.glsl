#version 330 core
in vec3 pass_color;
in vec3 pass_normal;
in vec3 pass_pos;
in vec2 pass_uv;

out vec4 out_color;

uniform vec3 lightColor;
uniform vec3 lightDir;
uniform float diffuseMin;
uniform float ambientStr;

void main() {
  vec3 ambient = ambientStr * lightColor;
  vec3 norm = normalize(pass_normal);
  float diff = max(dot(norm, lightDir), diffuseMin);
  vec3 diffuse = diff * lightColor;
  vec3 result = (ambient + diffuse) * pass_color;
  out_color = vec4(result, 1.0);
}
