#version 460 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTexCrds;

out vec3 fPos;
out vec2 fTexCrds;
out vec3 fNorm;

uniform mat4 modelmat;
uniform mat4 viewmat;
uniform mat4 projmat;

void main() {
    fPos = vPos;
    fTexCrds = vTexCrds;
    fNorm = vNorm;
    gl_Position = projmat*viewmat*modelmat*vec4(vPos, 1.0);
}