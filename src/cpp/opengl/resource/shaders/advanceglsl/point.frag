#version 330 core

out vec4 frag_color;

// 接口块(块名和输出一致都为VEC_OUT, 实例名可以更改为vin)
in VEC_OUT {
    vec3 pos;
} vin;

void main()
{
    // 记录了此片段在屏幕上的坐标(x, y, z(深度)), 左下角为原点
    if (gl_FragCoord.y <= 300) {
        frag_color = vec4(1, 1, 0, 1);
    } else {
        frag_color = vec4(0, 1, 1, 1);
    }
    // 记录了当前片段是正向面还是反向面
    if (gl_FrontFacing) {
        frag_color = frag_color - vec4(0, 1, 0, 0);
    } else {
        frag_color = frag_color - vec4(0, 0.5, 0, 0);
    }
    // 允许修改gl_FragCoord.z的值, 但是会禁用提前深度测试从而降低性能
    gl_FragDepth = 0.5;
}

