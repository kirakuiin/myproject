#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 atexcoords;

out vec3 fragpos;
out vec2 texcoords;
out vec3 normal;
out vec3 lightpos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_mat;
uniform vec3 alightpos;

void main()
{
    texcoords = atexcoords;
    normal = normalize(normal_mat * anormal);
    fragpos = (view * model * vec4(apos, 1.0)).xyz;
    lightpos = (view * model * vec4(alightpos, 1.0)).xyz;
    gl_Position = projection * view * model * vec4(apos, 1.0);
}
