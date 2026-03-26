#version 430

// Input and output topologies
layout(triangles) in;
layout(triangle_strip, max_vertices = 170) out;

layout(location = 0) in vec3 v_normals[];
layout(location = 1) in vec2 v_texture_coords[];

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
// TODO(student): Declare other uniforms here
uniform float time;

// Output
layout(location = 0) out vec2 texture_coord;

void EmitPoint(vec3 position, vec3 normal)
{
    texture_coord = v_texture_coords[0];
    gl_Position = Projection * View * vec4(position + normal * time, 1);
    EmitVertex();
}

void main()
{
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;
    vec3 p3 = gl_in[2].gl_Position.xyz;

    vec3 normal = cross(normalize(p2 - p1), normalize(p3 - p1));

    EmitPoint(p1, normal);
    EmitPoint(p2, normal);
    EmitPoint(p3, normal);

    EndPrimitive();
}
