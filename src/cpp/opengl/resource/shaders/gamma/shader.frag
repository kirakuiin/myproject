#version 330 core

in vec3 fragpos;
in vec2 texcoords;
in vec3 normal;
in vec3 lightpos;

out vec4 frag_color;

uniform sampler2D tex;
uniform bool isblinn;

vec4 calc_ambient(vec3 color) {
    float factor = 0.1;
    return vec4(factor * color, 0);
}

vec4 calc_diffuse(vec3 color) {
    vec3 lightdir = normalize(lightpos - fragpos);
    float factor = max(dot(normal, lightdir), 0);
    return vec4(factor * color, 0);
}

vec4 calc_phong(vec3 color) {
    vec3 viewdir = normalize(-fragpos);
    vec3 lightdir = normalize(lightpos - fragpos);
    vec3 reflectdir = reflect(-lightdir, normal);
    float factor = pow(max(dot(viewdir, reflectdir), 0), 8);
    return vec4(factor * color, 0);
}

vec4 calc_blinnphong(vec3 color) {
    vec3 viewdir = normalize(-fragpos);
    vec3 lightdir = normalize(lightpos - fragpos);
    vec3 halfway = normalize(viewdir + lightdir);
    float factor = pow(max(dot(normal, halfway), 0), 64);
    return vec4(factor * color, 0);
}

void main()
{
    vec4 color = texture(tex, texcoords).rgba;
    frag_color = vec4(0, 0, 0, color.a);
    frag_color += calc_ambient(color.xyz);
    frag_color += calc_diffuse(color.xyz);
    if (isblinn) {
        frag_color += calc_blinnphong(color.xyz);
    } else {
        frag_color += calc_phong(color.xyz);
    }
    float distance = abs(length(fragpos-lightpos));
    float gamma = 2.2;
    // 如果是漫反射类型贴图, 首先需要变换到srgb空间之后在进行校正
    frag_color.rgb = pow(frag_color.rgb, vec3(1.0/gamma));
}

