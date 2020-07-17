#version 330 core

in vec2 texcoords;

out vec4 frag_color;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedospec;

struct Light {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float radius;
};
const int NUM_LIGHTS = 3;
uniform Light lights[NUM_LIGHTS];
uniform vec3 view_pos;

void main()
{
    vec3 frag_pos = texture(g_position, texcoords).rgb;
    vec3 normal = texture(g_normal, texcoords).rgb;
    vec3 albedo = texture(g_albedospec, texcoords).rgb;
    float spec = texture(g_albedospec, texcoords).a;

    frag_color = vec4(0);
    float ambient = 0.2;
    vec3 view_dir = normalize(view_pos - frag_pos);
    for (int i = 0; i < NUM_LIGHTS; ++i) {
        float distance = length(lights[i].position - frag_pos);
        // 仅渲染光体积之内的片段
        if (distance <= lights[i].radius) {
            float attenuation = 1.0 / (lights[i].constant
                                       + lights[i].linear * distance
                                       + lights[i].quadratic * distance * distance);
            vec3 light_dir = normalize(lights[i].position - frag_pos);
            vec3 diffuse = max(0, dot(light_dir, normal)) * albedo ;
            vec3 halfway = normalize(view_dir + light_dir);
            float specular = pow(max(0, dot(normal, halfway)), 32) * spec;
            diffuse *= attenuation;
            specular *= attenuation;
            frag_color.xyz += diffuse*lights[i].color + specular*lights[i].color;
        }
    }
    frag_color.xyz += (albedo * ambient);
    frag_color.w = 1;
}

