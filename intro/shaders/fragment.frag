#version 460 core

in vec2 fTexCrds;

uniform sampler2D tex;

out vec4 color;

void main() {
    vec4 texColor = texture(tex, fTexCrds);
    if (texColor.a <= 0.1f) {
        discard;
    }
    color = texture(tex, fTexCrds);
}
