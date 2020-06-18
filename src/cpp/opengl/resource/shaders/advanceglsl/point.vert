#version 330 core

layout (location = 0) in vec3 apos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// 接口块
out VEC_OUT {
    vec3 pos;
} vout;

void main()
{
    vout.pos = apos;
    // 顶点着色器裁剪空间输出位置向量
    gl_Position = projection * view * model * vec4(apos, 1);
    // 设置顶点的大小(使用图元GL_POINTS)
    gl_PointSize = gl_Position.z;
    // 只读变量, 记录当前的索引或者是已经处理顶点的数量
    int index = gl_VertexID;
}
