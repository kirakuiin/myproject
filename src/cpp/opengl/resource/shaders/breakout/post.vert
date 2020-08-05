#version 330 core

layout (location = 0) in vec2 apos;
layout (location = 1) in vec2 atexcoords;

out vec2 texcoords;
out vec2 pos;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

void main()
{
    gl_Position = vec4(apos, 0.0, 1.0);
    pos = apos;
    if (chaos) {
        float strength = 0.3;
        texcoords = vec2(atexcoords.x + sin(time) * strength,
                        atexcoords.y + cos(time) * strength);
    } else if (confuse) {
        texcoords = vec2(1.0 - atexcoords.x, 1.0 - atexcoords.y);
    } else {
        texcoords = atexcoords;
    }
    if (shake) {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;
        gl_Position.y += cos(time * 15) * strength;
    }
}
