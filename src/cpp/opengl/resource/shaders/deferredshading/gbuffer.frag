#version 330 core

layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_albedospec;

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
} vs_in;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;

void main()
{
    g_position = vs_in.frag_pos;
    g_normal = normalize(vs_in.normal);
    g_albedospec.rgb = texture(texture_diffuse0, vs_in.texcoords).rgb;
    g_albedospec.a = texture(texture_specular0, vs_in.texcoords).r;
}
