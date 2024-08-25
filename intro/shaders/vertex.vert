#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 fColor;

void main() {
    gl_Position = projMat*viewMat*modelMat*vec4(vPos, 1.0f);

    fColor = vColor;
}