#version 330 core

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
} vs_in;

out vec4 frag_color;

uniform sampler2D wall;

uniform vec3 light_posis[4];
uniform vec3 light_colors[4];
uniform vec3 view_pos;

vec3 CalcPhongLighting(int index, vec3 viewdir) {
    vec3 light_pos = light_posis[index];
    vec3 light_color = light_colors[index];
    vec3 lightdir = normalize(light_pos - vs_in.frag_pos);

    vec3 ambient = light_color * 0.1;
    vec3 diffuse = light_color * max(0, dot(vs_in.normal, lightdir));
    vec3 halfway = normalize(viewdir + lightdir);
    vec3 specular = light_color * pow(max(0, dot(vs_in.normal, halfway)), 32);

    float distance = length(light_pos - vs_in.frag_pos);
    float attenuation = 1.0/(1 + distance*0.22 + distance*distance*0.20);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 viewdir = normalize(view_pos - vs_in.frag_pos);
    vec3 factor = vec3(0, 0, 0);
    for (int i = 0; i < 4; ++i) {
        factor += CalcPhongLighting(i, viewdir);
    }
    frag_color = texture(wall, vs_in.texcoords);
    frag_color.xyz = factor * frag_color.xyz;
}
