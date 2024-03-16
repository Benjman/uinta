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

// NOTE: These values need to match the values defined in `shaders/primitive.h`
const int SwayMask = 1 << 0;
const int UvMask = 1 << 1;

void main() {
  vec4 uv = (uFlags & UvMask) != 0
    ? texture(uTexture, pUv)
    : vec4(1);
  vec3 ambient = uAmbientStr * uLightColor;
  vec3 lightDir = normalize(-uLightDir);
  float diff = max(dot(pNormal, lightDir), uDiffuseMin);
  vec3 diffuse = diff * uLightColor;
  vec3 result = (ambient + diffuse) * pColor;
  oColor =  vec4(result, 1.0) * uv;
}
