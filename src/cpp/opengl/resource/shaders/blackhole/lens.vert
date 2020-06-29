#version 330 core

layout (location = 0) in vec3 apos;

out vec3 pos;
out vec3 normal;
out vec3 O;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal_mat;
uniform vec3 origin;

void main()
{
    normal = normalize(normal_mat * apos);
    pos = vec3(model * vec4(apos, 1));
    O = vec3(model * vec4(origin, 1));
    gl_Position = projection * view * model * vec4(apos, 1);
}
