#version 330 core

layout (location = 0) in vec3 apos;

uniform mat4 model;
uniform mat4 lightmat;

void main()
{
    gl_Position = lightmat * model * vec4(apos, 1);
}
