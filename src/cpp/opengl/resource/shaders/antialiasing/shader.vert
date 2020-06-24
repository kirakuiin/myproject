#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 acolor;

out vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    color = acolor;
    gl_Position = projection * view * model * vec4(apos, 1);
}
