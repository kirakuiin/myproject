#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 atexcoords;

out VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(apos, 1);

    mat3 normal_mat = transpose(inverse(mat3(view * model)));
    vs_out.frag_pos = vec3(view * model * vec4(apos, 1));
    vs_out.normal = normal_mat * anormal;
    vs_out.texcoords = atexcoords;
}
