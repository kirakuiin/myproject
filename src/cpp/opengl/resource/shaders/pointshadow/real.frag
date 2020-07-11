#version 330 core

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
} vs_in;

out vec4 frag_color;

uniform sampler2D tex;
uniform samplerCube depth_map;

uniform vec3 light_pos;
uniform vec3 view_pos;

uniform float far_plane;

float undershadow() {
    vec3 light_to_frag = vs_in.frag_pos - light_pos;
    float closestdepth = texture(depth_map, normalize(light_to_frag)).r;
    closestdepth *= far_plane;
    float currentdepth = length(light_to_frag);
    float shadow = currentdepth > closestdepth ? 0 : 1;
    return shadow;
}

void main()
{
    float ambient = 0.2;
    vec3 lightdir = normalize(light_pos - vs_in.frag_pos);
    float diffuse = max(0, dot(vs_in.normal, lightdir));
    vec3 viewdir = normalize(view_pos - vs_in.frag_pos);
    vec3 halfway = normalize(viewdir + lightdir);
    float specular = pow(max(0, dot(vs_in.normal, halfway)), 32);
    frag_color = texture(tex, vs_in.texcoords);
    float shadow = undershadow();
    frag_color.xyz = frag_color.xyz*(ambient + shadow*(diffuse + specular));
}
