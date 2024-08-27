#version 460 core
out vec4 color;

in vec3 fPos;
in vec2 fTexCrds;
in vec3 fNorm;

// for textures
uniform int hasTexture;
uniform sampler2D texture_diffuse1;

// for material lighting
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float ns;

// light position
uniform vec3 lightPos;

// camera position
uniform vec3 camPos;

float Ia = 0.3;
float Il = 1.0;

void main() {    
    vec3 N = normalize(fNorm);
    vec3 L = normalize(lightPos - fPos);
    vec3 O = normalize(camPos - fPos);
    vec3 R = reflect(-L, N);

    vec3 C = ka*Ia + Il*(kd*abs(dot(N,L)) + ks*pow(abs(dot(O,R)), ns));
    if (hasTexture == 1) {
        color = texture(texture_diffuse1, fTexCrds)*vec4(C,1.f);
    } else {
        color = vec4(C,1.f);
    }
}