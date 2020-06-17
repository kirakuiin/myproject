#version 330 core

in vec2 texcoords;

out vec4 frag_color;

uniform sampler2D tex;

void main()
{
    const float offset = 1.0/300;
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),
        vec2(0, offset),
        vec2(offset, offset),
        vec2(-offset, 0),
        vec2(0, 0),
        vec2(offset, 0),
        vec2(-offset, -offset),
        vec2(0, -offset),
        vec2(offset, -offset)
    );

    float kernal[9] = float[](
        0.0625, 0.125, 0.0625,
        0.125, 0.25, 0.125,
        0.0625, 0.125, 0.0625
    );

    vec3 sample_tex[9];
    for (int i = 0; i < 9; ++i) {
        sample_tex[i] = texture(tex, texcoords.st+offsets[i]).rgb;
    }
    vec3 col = vec3(0);
    for (int i = 0; i < 9; ++i) {
        col += sample_tex[i] * kernal[i];
    }
    frag_color = vec4(col, 1.0);
}

