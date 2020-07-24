#version 330 core

in vec2 texcoords;

out vec4 frag_color;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D tex_noise;

uniform vec3 samples[64];
uniform mat4 projection;

// 这里假定噪声纹理的大小为4x4
const vec2 noise_scale = vec2(800.0/4, 600.0/4);
const int kernel_size = 64;
const float radius = 1.0;
const float bias = 0.025;

void main()
{
    vec3 frag_pos = texture(g_position, texcoords).xyz;
    vec3 normal = texture(g_normal, texcoords).rgb;
    vec3 random_vec = texture(tex_noise, texcoords * noise_scale).xyz;

    vec3 tangent = normalize(random_vec - normal * dot(random_vec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for (int i = 0; i < kernel_size; ++i) {
        // 变换到观察空间
        vec3 sample = TBN * samples[i];
        sample = frag_pos + sample * radius;

        // 变换到裁剪空间
        vec4 offset = vec4(sample, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w; // 透视除法
        offset.xyz = offset.xyz * 0.5 + 0.5; // 变换到ndc坐标

        float sample_depth = texture(g_position, offset.xy).z;
        float range_check = smoothstep(0.0, 1.0, radius/abs(frag_pos.z-sample_depth));
        occlusion += (sample_depth >= sample.z+bias ? 1.0 : 0.0) * range_check;
    }
    occlusion = 1.0 - (occlusion / kernel_size);
    frag_color = vec4(occlusion);
}

