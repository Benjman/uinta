#version 330 core

in vec3 pColor;
in vec3 pNormal;
in vec3 pPos;
in vec2 pUv;

out vec4 oColor;

uniform int uFlags;
uniform vec3 uLightColor;
uniform vec3 uLightDir;
uniform float uDiffuseMin;
uniform float uAmbientStr;

uniform sampler2D uTexture;

bool isSwayEnabled(int flags) {
  const int SwayMask = 1 << 0;
  return (flags & SwayMask) != 0;
}

bool isUvEnabled(int flags) {
  const int UvMask = 1 << 1;
  return (flags & UvMask) != 0;
}

bool isNormalsEnabled(int flags) {
  const int NormalMask = 1 << 2;
  return (flags & NormalMask) != 0;
}


void main() {
  vec3 ambient = uAmbientStr * uLightColor;
  vec3 lightDir = normalize(-uLightDir);
  float diff = max(dot(pNormal, lightDir), uDiffuseMin);
  vec3 diffuse = diff * uLightColor;
  vec3 color = (ambient + diffuse) * pColor;

  if (isNormalsEnabled(uFlags)) {
    const float dimFactor = 0.66;
    color = abs(pNormal) * dimFactor + max(pNormal, vec3(0.0));
  }

  oColor = vec4(color, 1.0);

  if (isUvEnabled(uFlags)) oColor *= texture(uTexture, pUv);
}
