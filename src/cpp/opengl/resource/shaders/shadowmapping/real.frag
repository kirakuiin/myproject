#version 330 core

in VS_OUT {
    vec2 texcoords;
    vec3 normal;
    vec3 frag_pos;
    vec4 light_view;
} vs_in;

out vec4 frag_color;

uniform sampler2D tex;
uniform sampler2D shadow;

uniform vec3 lightdir;
uniform vec3 view_pos;

float undershadow(void) {
    vec4 lightview = vs_in.light_view;
    lightview.xyz = lightview.xyz/lightview.w;
    lightview = lightview * 0.5 + 0.5;
    vec2 texsize = 1.0/textureSize(shadow, 0);
    float bias = 0;//max(0.005, 0.05*(1-dot(vs_in.normal, -lightdir)));
    float shadow_v = 1;
    if (lightview.z >= 1.0) {
        return shadow_v;
    }
    if (lightview.z - bias > texture(shadow, lightview.xy).r) {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1;  j <= 1; ++j) {
                float pcfdepth = texture(
                        shadow, lightview.xy + vec2(i, j) * texsize).r;
                shadow_v += (lightview.z - bias > pcfdepth ? 0 : 1);
            }
        }
        shadow_v /= 9.0;
    }
    return shadow_v;
}

void main()
{
    float ambient = 0.1;
    float diffuse = max(0, dot(vs_in.normal, -lightdir));
    vec3 viewdir = normalize(view_pos - vs_in.frag_pos);
    vec3 halfway = normalize(viewdir - lightdir);
    float specular = pow(max(0, dot(vs_in.normal, halfway)), 32);
    frag_color = texture(tex, vs_in.texcoords);
    frag_color.xyz = frag_color.xyz*(ambient + undershadow()*(diffuse + specular));
}
