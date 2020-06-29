#version 330 core

in vec3 texcoords;

out vec4 frag_color;

uniform samplerCube universe;

void main()
{
    frag_color = texture(universe, texcoords);
}
