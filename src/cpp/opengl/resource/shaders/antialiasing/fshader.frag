#version 330 core

in vec2 texcoords;

out vec4 frag_color;

uniform sampler2D tex;

void main()
{
    // 如果使用uniform sampler2DMS直接处理多采样缓纹理的话,
    // 可以使用texelFetch函数来获得每个采样的颜色值, 自定义反锯齿算法
    frag_color = texture(tex, texcoords);
}
