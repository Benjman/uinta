#version 330 core
layout (location = 0) in vec2 in_pos;
out vec3 pass_color;
void main() {
   gl_Position = vec4(in_pos.x, in_pos.y, 1.0, 1.0);
};