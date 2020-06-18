#version 330 core

out vec4 frag_color;

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
};
uniform Material material;
uniform samplerCube skybox;

in vec3 normal;
in vec2 texcoords;
in vec3 position;

void main()
{
    float radio = 1/1.52;
    vec3 viewdir = normalize(position);
    vec3 refdir = refract(viewdir, normalize(normal), radio);
    frag_color = texture(skybox, refdir);
}
