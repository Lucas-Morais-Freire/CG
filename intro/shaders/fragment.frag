#version 460 core

in vec3 fColor;
out vec4 color;

void main() {
    color.rgb = fColor;
    color.a = 1.0f;
}
