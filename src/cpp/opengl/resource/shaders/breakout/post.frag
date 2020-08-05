#version 330 core

in vec2 texcoords;
in vec2 pos;
out vec4 color;

uniform sampler2D scene;
uniform vec2 offsets[9];
uniform int edge_kernel[9];
uniform float blur_kernel[9];

uniform float time;
uniform float begin_time;
uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform bool timestop;

void main() {
    color = vec4(0.0f);
    vec3 sample[9];
    if (chaos || shake) {
        for (int i = 0; i < 9; ++i) {
            sample[i] = vec3(texture(scene, texcoords + offsets[i]).rgb);
        }
    }
    if (chaos) {
        for (int i = 0; i < 9; ++i) {
            color += vec4(sample[i] * edge_kernel[i], 0);
        }
        color.a = 1.0f;
    } else if (confuse) {
        color = vec4(1.0 - texture(scene, texcoords).rgb, 1.0);
    } else if (shake)  {
        for (int i = 0; i < 9; ++i) {
            color += vec4(sample[i] * blur_kernel[i], 0);
        }
        color.a = 1.0f;
    } else {
        color = texture(scene, texcoords);
    }
    if (timestop) {
        float delta = time-begin_time;
        if (pos.x*pos.x / 9 + pos.y*pos.y / 16 <= 0.05*delta*8) {
            color = vec4(1.0 - color.rgb, 1.0);
        }
    }
}
