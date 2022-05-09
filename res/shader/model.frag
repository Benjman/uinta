#version 330 core

in vec3 pass_norm;
in vec3 pass_frag_pos;

out vec4 out_color;

const vec3 obj_color = vec3(1.0, 0.3, 0.3);

// light info
const vec3 light_pos = vec3(0, 0, -5);
const vec3 light_dir = normalize(vec3(0, 0, -1));
const vec3 light_color = vec3(1);

// ambient lighting
const float ambient_str = 0.1;
const vec3 ambient = ambient_str * light_color;

void main() {
  vec3 norm = normalize(pass_norm);
  vec3 lightDir = normalize(light_pos - pass_frag_pos);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * light_color;

  out_color = vec4((ambient + diffuse) * obj_color, 1.0);
}
