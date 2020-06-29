#version 330 core

in vec3 pos;
in vec3 normal;
in vec3 O;

out vec4 frag_color;

uniform samplerCube universe;
uniform vec3 vpos;

void main()
{
    float ratio = 1/1.52;
    vec3 R = refract(normalize(pos-vpos), normal, ratio);
    frag_color = vec4(texture(universe, R).rgb, 1);
}

