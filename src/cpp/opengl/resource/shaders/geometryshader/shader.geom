#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices=6) out;

out vec3 color;

in VS_OUT {
    vec3 color;
    vec4 normal;
} gs_in[] ;

void GenerateLine(int idx) {
    vec4 pos = gl_in[idx].gl_Position;
    vec4 normal = normalize(gs_in[idx].normal);
    color = gs_in[idx].color;
    gl_Position = pos;
    EmitVertex();
    gl_Position = pos + normal*0.5;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    for (int i = 0; i < 3; ++i) {
        GenerateLine(i);
    }
}
