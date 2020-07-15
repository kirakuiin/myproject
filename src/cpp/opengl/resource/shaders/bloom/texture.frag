#version 330 core

in vec2 texcoords;

out vec4 frag_color;

uniform float exposure;

uniform sampler2D tex;
uniform sampler2D flur;

void main()
{
    const float gamma = 2.2;
    vec3 hdr = texture(tex, texcoords).rgb;
    vec3 flur = texture(flur, texcoords).rgb;
    hdr += flur;

    // 曝光色调映射
    vec3 mapped = vec3(1) - exp(-hdr * exposure);
    // vec3 mapped = hdr / (hdr + vec3(1.0));

    mapped = pow(mapped, vec3(1.0/gamma));
    frag_color = vec4(mapped, 1.0);

}

