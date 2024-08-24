#version 460 core

in vec3 fColor;
in vec2 fTexCoord;

uniform sampler2D tex;

out vec4 color;

void main() {
    color = texture(tex, fTexCoord) * vec4(fColor, 1.0);
}
