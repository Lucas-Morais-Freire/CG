#version 460 core

layout (location = 0) in vec4 wPos;

void main() {
    gl_Position = wPos;
}