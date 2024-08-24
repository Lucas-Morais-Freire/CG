#version 460 core

in vec3 fColor;
out vec4 color;

uniform float brightness;

void main() {
    color.rgb = brightness*fColor;
    color.a = 1.0f;
}
