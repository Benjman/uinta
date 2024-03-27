#version 330 core
layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec2 in_texCoord;

out vec2 pass_texCoord;

uniform mat4 uProjection = mat4(1);

void main() {
  gl_Position = uProjection * vec4(in_pos, -1.0, 1.0);
  pass_texCoord = vec2(in_texCoord.x, in_texCoord.y);
}
