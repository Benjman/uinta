#version 330 core

in vec3 pass_color;
in vec3 pass_norm;

const vec3 light_color = vec3(1.0);
const vec3 light_dir = normalize(vec3(0, 0, -1));
const float ambient_str = 0.1;

out vec4 out_color;

void main() {
  vec3 ambient = ambient_str * light_color;
  vec3 diffuse = max(dot(pass_norm, light_dir), 0.0) * light_color;
  out_color = vec4((ambient + diffuse) * pass_color, 1.0);
}
