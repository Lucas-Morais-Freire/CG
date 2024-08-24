#version 460 core

in vec3 fColor;
in vec2 fTexCoord;

uniform sampler2D tex1;
// uniform sampler2D tex2;

uniform float brightness;

out vec4 color;

void main() {
    color = brightness*texture(tex1, fTexCoord);
}
