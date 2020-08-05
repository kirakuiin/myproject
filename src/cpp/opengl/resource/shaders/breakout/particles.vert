#version 330 core

layout (location = 0) in vec2 apos;
layout (location = 1) in vec2 atexcoords;

out vec2 texcoords;

uniform mat4 projection;
uniform vec2 offset;

void main()
{
    float scale = 10.0;
    texcoords = atexcoords;
    gl_Position = projection * vec4((apos * scale) + offset, 0.0, 1.0);
}
