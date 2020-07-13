#version 330 core

in VS_OUT {
    vec2 texcoords;
    vec3 frag_pos;
    vec3 tangent_frag_pos;
    vec3 tangent_light_pos;
    vec3 tangent_view_pos;
} vs_in;

out vec4 frag_color;

uniform sampler2D brick;
uniform sampler2D normal;

uniform bool islight;

void main()
{
    float ambient = 0.2;
    vec3 lightdir = normalize(vs_in.tangent_light_pos - vs_in.tangent_frag_pos);
    vec3 viewdir = normalize(vs_in.tangent_view_pos - vs_in.tangent_frag_pos);
    vec3 normaldir = texture(normal, vs_in.texcoords).rgb;
    normaldir = normaldir*2-1;
    normaldir.g = -normaldir.g;
    float diffuse = max(0, dot(normaldir, lightdir));
    vec3 halfway = normalize(viewdir + lightdir);
    float specular = pow(max(0, dot(normaldir, halfway)), 32);
    if (islight) {
        frag_color = vec4(1);
    } else {
        frag_color = texture(brick, vs_in.texcoords);
        frag_color = (ambient + specular + diffuse) * frag_color;
    }
}
