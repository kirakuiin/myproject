#version 330 core

in vec2 texcoords;

out vec4 frag_color;

uniform sampler2D tex;

float linearize_depth(float depth) {
    float near = 0.1;
    float far = 100.0;
    float z = depth * 2.0 - 1.0;
    return ((2.0 * near * far) / (far + near - z*(far - near)))/far;
}

void main()
{
    frag_color = vec4(vec3(linearize_depth(gl_FragCoord.z)), 1.0);
}

