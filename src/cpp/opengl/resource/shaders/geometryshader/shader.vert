#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 acolor;
layout (location = 2) in vec3 anormal;

out VS_OUT {
    vec3 color;
    vec4 normal;
} v_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    v_out.color = acolor;
    // 法向量必须也要变换到裁剪空间，需要保留w
    v_out.normal = projection * transpose(inverse(view * model)) * vec4(anormal, 0);
    gl_Position = projection * view * model * vec4(apos, 1);
}
