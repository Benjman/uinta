#version 330 core
out vec4 out_color;

in vec2 pass_uv;

uniform sampler2D screenTexture;

void main() {
    vec3 col = texture(screenTexture, pass_uv).rgb;
    out_color = vec4(1 - col, 1.0);
    // float average = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
    // out_color = vec4(vec3(average), 1.0);
}
