#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 atexcoords;

out vec2 texcoords;
out vec3 normal;
out vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_mat;

void main()
{
    normal = normal_mat * anormal;
    texcoords = atexcoords;
    position = vec3(view * model * vec4(apos, 1));
    gl_Position = projection * view * model * vec4(apos, 1.0);
}
