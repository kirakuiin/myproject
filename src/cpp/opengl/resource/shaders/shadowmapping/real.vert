#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 atexcoords;

out VS_OUT {
    vec2 texcoords;
    vec3 normal;
    vec3 frag_pos;
    vec4 light_view;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightmat;
uniform mat3 normal_mat;

void main()
{
    vs_out.normal = normal_mat * anormal;
    vs_out.light_view = lightmat * model * vec4(apos, 1);
    vs_out.texcoords = atexcoords;
    vs_out.frag_pos = vec3(model * vec4(apos, 1));
    gl_Position = projection * view * model * vec4(apos, 1);
}
