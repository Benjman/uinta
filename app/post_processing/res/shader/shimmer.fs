#version 330 core

in vec2 pass_uv;

out vec3 out_color;

uniform sampler2D renderedTexture;
uniform float time = 1;
uniform float scale = 0.001;

void main(){
  out_color = texture(renderedTexture, pass_uv + scale * vec2(sin(time + 1920.0 * pass_uv.x), cos(time + 1080.0 * pass_uv.y))).xyz;
}
