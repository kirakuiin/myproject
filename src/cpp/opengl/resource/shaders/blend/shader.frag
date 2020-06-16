#version 330 core

in vec2 texcoords;

out vec4 frag_color;

uniform sampler2D tex;

void main()
{
    vec4 texcolor = texture(tex, texcoords);
    frag_color = texcolor;
}

