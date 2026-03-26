#version 430

// Input and output topologies
layout(triangles) in;
layout(line_strip, max_vertices = 170) out;

layout(location = 0) in vec3 v_normals[];

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
// TODO(student): Declare other uniforms here
uniform float time;
uniform float amplitude;

// Output
layout(location = 0) out vec3 color;

void EmitLine(vec3 p1, vec3 p2)
{
    color = vec3(0, 0, 1);
    gl_Position = Projection * View * vec4(p1, 1);
    EmitVertex();

    color = vec3(0, 0, 1);
    gl_Position = Projection * View * vec4(p2, 1);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;
    vec3 p3 = gl_in[2].gl_Position.xyz;

    vec3 normal1 = v_normals[0];
    vec3 normal2 = v_normals[1];
    vec3 normal3 = v_normals[2];

    EmitLine(p1, p1 + normal1 * amplitude);
    EmitLine(p2, p2 + normal2 * amplitude);
    EmitLine(p3, p3 + normal3 * amplitude);
}
