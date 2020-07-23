#version 330 core

in vec2 texcoords;

out vec4 frag_color;

uniform sampler2D ssao;

void main()
{
    vec2 texel_size = 1.0 / vec2(textureSize(ssao, 0));
    float result = 0;
    for (int x = -2; x < 2; ++x) {
        for (int y = -2; y < 2; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texel_size;
            result += texture(ssao, texcoords + offset).r;
        }
    }
    frag_color = vec4(result / (4.0*4.0));
}
