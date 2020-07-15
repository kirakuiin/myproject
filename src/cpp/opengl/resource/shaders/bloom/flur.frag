#version 330 core

in vec2 texcoords;

out vec4 frag_color;

uniform sampler2D tex;

uniform bool ishorizontal;

uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    // gaussian flur(高斯模糊，权重采样)
    vec2 tex_offset = 1.0 / textureSize(tex, 0);
    vec3 result = texture(tex, texcoords).rgb * weight[0];
    if (ishorizontal) {
        for (int i = 1; i < 5; ++i) {
            result += texture(tex, texcoords+vec2(i*tex_offset.x, 0)).rgb * weight[i];
            result += texture(tex, texcoords-vec2(i*tex_offset.x, 0)).rgb * weight[i];
        }
    } else {
        for (int i = 1; i < 5; ++i) {
            result += texture(tex, texcoords+vec2(0, i*tex_offset.y)).rgb * weight[i];
            result += texture(tex, texcoords-vec2(0, i*tex_offset.y)).rgb * weight[i];
        }
    }
    frag_color = vec4(result, 1);
}


