#version 330 core

in vec2 texcoords;

out vec4 frag_color;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedospec;
uniform sampler2D ssao;
uniform bool use_ssao ;

struct Light {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

void main()
{
    vec3 frag_pos = texture(g_position, texcoords).rgb;
    vec3 normal = texture(g_normal, texcoords).rgb;
    vec3 albedo = texture(g_albedospec, texcoords).rgb;
    float occlusion = texture(ssao, texcoords).r;
    frag_color = vec4(0);
    if (use_ssao) {
        frag_color = vec4(occlusion);
    } else {
        // blinn-phong
        float ambient = 0.2 * occlusion;
        vec3 view_dir = normalize(-frag_pos);
        vec3 light_dir = normalize(light.position - frag_pos);
        vec3 diffuse = max(0, dot(light_dir, normal)) * albedo;
        vec3 halfway = normalize(view_dir + light_dir);
        float specular = pow(max(0, dot(normal, halfway)), 8);

        // attenuation
        float distance = length(light.position - frag_pos);
        float attenuation = 1.0 / (light.constant
                + light.linear * distance
                + light.quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        frag_color.xyz += diffuse*light.color + specular*light.color;
        frag_color.xyz += (albedo * ambient);
        frag_color.w = 1;
    }
}
