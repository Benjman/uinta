#version 330 core

in vec3 pColor;
in vec3 pPos;

out vec4 oColor;

void main() {
  bvec3 s = greaterThanEqual(pPos, vec3(0.0));
  vec3 color;
  if (s.x && s.y && s.z) color = vec3(1.0, 0.0, 0.0);
  else if (!s.x && s.y && s.z) color = vec3(0.0, 1.0, 0.0);
  else if (s.x && !s.y && s.z) color = vec3(0.0, 0.0, 1.0);
  else if (s.x && s.y && !s.z) color = vec3(1.0, 1.0, 0.0);
  else if (!s.x && !s.y && s.z) color = vec3(1.0, 0.0, 1.0);
  else if (!s.x && s.y && !s.z) color = vec3(0.0, 1.0, 1.0);
  else if (s.x && !s.y && !s.z) color = vec3(1.0, 0.5, 0.0);
  else color = vec3(1.0, 1.0, 1.0);

  // Border lines along the x=0, y=0, z=0 planes (pixel-stable via fwidth).
  vec3 d = abs(pPos) / max(fwidth(pPos), vec3(1e-5));
  float edge = min(min(d.x, d.y), d.z);
  float line = 1.0 - smoothstep(0.25, 0.5, edge);
  color = mix(color, vec3(0.0), line);

  oColor = vec4(color, 1.0);
}
