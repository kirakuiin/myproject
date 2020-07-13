#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 atexcoords;
layout (location = 3) in vec3 atangent;
layout (location = 4) in vec3 abitangent;

out VS_OUT {
    vec2 texcoords;
    vec3 frag_pos;
    vec3 tangent_frag_pos;
    vec3 tangent_light_pos;
    vec3 tangent_view_pos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{
    mat3 normal_mat = mat3(transpose(inverse(model)));
    vec3 T = normalize(normal_mat * atangent);
    vec3 B = normalize(normal_mat * abitangent);
    vec3 N = normalize(normal_mat * anormal);
    // 对于正交矩阵来说矩阵的逆的矩阵的转置
    mat3 tangent_mat = transpose(mat3(T, B, N));
    vs_out.texcoords = atexcoords;
    vs_out.frag_pos = vec3(model * vec4(apos, 1));
    vs_out.tangent_frag_pos = tangent_mat * vs_out.frag_pos;
    vs_out.tangent_view_pos = tangent_mat * view_pos;
    vs_out.tangent_light_pos = tangent_mat * light_pos;
    gl_Position = projection * view * model * vec4(apos, 1);
}
