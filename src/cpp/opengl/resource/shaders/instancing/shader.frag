#version 330 core

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
};

uniform Material material;

in vec3 normal;
in vec2 texcoords;

out vec4 frag_color;

void main()
{
    frag_color = texture(material.texture_diffuse0, texcoords);
}
