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
uniform sampler2D depth;

uniform bool islight;
uniform bool ispara;
uniform float height_scale;

vec2 ParallaxMapping(vec2 texcoords, vec3 viewdir) {
    // calc layer constant;
    const float min_layers = 8;
    const float max_layers = 32;
    float num_layers = mix(max_layers, min_layers,
                           abs(dot(vec3(0.0, 0.0, 1.0), viewdir)));
    float layer_depth = 1.0 / num_layers;
    float current_depth = 0.0;
    vec2 P = viewdir.xy * height_scale;
    vec2 delta = P / num_layers;

    vec2 current_coords = texcoords;
    float current_v = texture(depth, current_coords).r;
    while (current_depth < current_v) {
        current_coords -= delta;
        current_v = texture(depth, current_coords).r;
        current_depth += layer_depth;
    }
    // liner interpolation
    vec2 prev_coords = current_coords + delta;
    float prev_depth = current_depth - layer_depth;
    float prev_v = texture(depth, prev_coords).r;
    float before_v = abs(prev_v - prev_depth);
    float after_v = abs(current_v - current_depth);
    float weight = after_v / (after_v + before_v);
    vec2 final_coords = prev_coords * weight + current_coords * (1 - weight);
    return final_coords;
}

void main()
{
    float ambient = 0.2;
    vec3 lightdir = normalize(vs_in.tangent_light_pos - vs_in.tangent_frag_pos);
    vec3 viewdir = normalize(vs_in.tangent_view_pos - vs_in.tangent_frag_pos);
    vec2 texcoords;
    if (ispara) {
        texcoords = ParallaxMapping(vs_in.texcoords, viewdir);
        if (texcoords.x > 1.0 || texcoords.y > 1.0 || texcoords.x < 0.0 || texcoords.y < 0.0) {
            discard;
        }
    } else {
        texcoords = vs_in.texcoords;
    }
    vec3 normaldir = texture(normal, texcoords).rgb;
    normaldir = normaldir*2-1;
    normaldir.g = -normaldir.g;
    float diffuse = max(0, dot(normaldir, lightdir));
    vec3 halfway = normalize(viewdir + lightdir);
    float specular = pow(max(0, dot(normaldir, halfway)), 32);
    if (islight) {
        frag_color = vec4(1);
    } else {
        frag_color = texture(brick, texcoords);
        frag_color = (ambient + specular + diffuse) * frag_color;
    }
}
