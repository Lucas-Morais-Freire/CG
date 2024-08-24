#version 460 core

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexCoord;

out vec3 fColor;
out vec2 fTexCoord;

void main() {
    gl_Position.xy = vPos;
    gl_Position.zw = vec2(0.0f, 1.0f);

    fColor = vColor;
    fTexCoord = vTexCoord;
}