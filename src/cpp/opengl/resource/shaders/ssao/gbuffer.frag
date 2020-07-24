#version 330 core

layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec3 g_albedospec;

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
} vs_in;

uniform float NEAR = 0.1;
uniform float FAR = 75.0;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;    // ndc
    // 根据透视矩阵推导出来的反向变换公式
    float real = (2.0 * NEAR * FAR) / (FAR + NEAR - z*(FAR - NEAR));
    return (real - NEAR) / (FAR - NEAR);
}

void main()
{
    g_position = vs_in.frag_pos;
    g_normal = normalize(vs_in.normal);
    g_albedospec = vec3(0.95);
}
