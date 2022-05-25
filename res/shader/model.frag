#version 330 core

in vec3 pass_norm;
in vec3 pass_frag_pos;

out vec4 out_color;

const vec3 obj_color = vec3(1.0, 0.3, 0.3);

// light info
const vec3 light_dir = normalize(vec3(0, 0, -1));
const vec3 light_color = vec3(1);

// ambient lighting
const float ambient_str = 0.1;
const vec3 ambient = ambient_str * light_color;

void main() {
  vec3 diffuse = max(dot(pass_norm, light_dir), 0.0) * light_color;
  out_color = vec4((ambient + diffuse) * obj_color, 1.0);
}
