#version 330 core

in vec2 texcoords;

out vec4 frag_color;

uniform sampler2D text;
uniform vec3 texcolor;

void main()
{
    vec4 sampled = vec4(1, 1, 1, texture(text, texcoords).r);
    frag_color = vec4(texcolor, 1.0) * sampled;
}
