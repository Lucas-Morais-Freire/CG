#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexCrds;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec2 fTexCrds;

void main() {
    gl_Position = projMat*viewMat*modelMat*vec4(vPos, 1.0f);

    fTexCrds = vTexCrds;
}