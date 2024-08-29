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

float Ia = 0.2f;
float Il = 1.6f;

float kc = 1.f;
float kl = 0.014;
float kq = 0.0007;

void main() {
    float d = length(lightPos - fPos);
    float fatt = 1.0f/(kc + kl*d + kq*d*d);
    // float fatt = 1.f;

    vec3 N = normalize(fNorm);
    vec3 L = normalize(lightPos - fPos);
    vec3 O = normalize(camPos - fPos);
    vec3 R = reflect(-L, N);

    vec3 C = ka*Ia + fatt*Il*(kd*max(dot(N,L), 0.f) + ks*pow(max(dot(O,R),0.f), ns));
    if (hasTexture == 1) {
        color = texture(texture_diffuse1, fTexCrds)*vec4(C,1.f);
    } else {
        color = vec4(C,1.f);
    }

    // color = vec4(normalize(fNorm), 1.f);
}