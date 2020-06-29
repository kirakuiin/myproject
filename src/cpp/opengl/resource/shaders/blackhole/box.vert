#version 330 core

layout (location = 0) in vec3 apos;

out vec3 texcoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    texcoords = apos;
    gl_Position = projection * view * model * vec4(apos, 1);
    // 将深度设置为1, 让贴图永远在其他物体之后
    gl_Position = gl_Position.xyww;
}
