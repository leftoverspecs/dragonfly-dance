#version 330 core
in vec2 frag_position;
in vec2 frag_texture_coord;

uniform sampler2D source_texture;
uniform float gamma;
uniform float exposure;
uniform float time;

struct bubble {
    vec2 position;
    float radius;
};

layout (std140) uniform bubbles {
    int size;
    float data[3 * 1000];
};

out vec4 FragColor;

#define M_PI 3.14159265358979323846

void main() {
    float phase = 2 * M_PI * time / 2000.0;
    vec3 color = texture(source_texture, frag_texture_coord).rgb;
    for (int i = 0; i < size; ++i) {
        vec2 pos = vec2(data[3 * i], data[3 * i + 1]);
        float radius = data[3 * i + 2];

        vec2 delta = frag_texture_coord - pos;
        float phi = atan(delta.x, delta.y);
        float offset = (0.02 * sin(4 * phi + 0.1 * phase / radius) + 0.02 * sin(2 * phi - 0.1 * phase / radius)) + 1.0;
        float frac = length(delta) / (offset * radius);
        if (frac < 1.0) {
            float fact = (exp(10 * frac) - 1) / (exp(10) - 1);
            color += vec3(fact, fact, fact);
            break;
        }
    }
    vec3 result = vec3(1.0) - exp(-color * exposure);
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}
