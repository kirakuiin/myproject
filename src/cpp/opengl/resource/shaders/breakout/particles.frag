#version 330 core

out vec4 particle_color;

in vec2 texcoords;

uniform sampler2D sprite;
uniform vec4 color;

void main()
{
    particle_color = texture(sprite, texcoords) * color;
}
