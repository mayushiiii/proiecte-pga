#version 430

// Input and output topologies
layout(triangles) in;
layout(triangle_strip, max_vertices = 170) out;

// Input
layout(location = 1) in vec2 v_texture_coord[];

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
// TODO(student): Declare other uniforms here
uniform int instances;

// Output
layout(location = 0) out vec2 texture_coord;

void EmitPoint(vec3 pos, vec3 offset)
{
    gl_Position = Projection * View * vec4(pos + offset, 1.0);
    EmitVertex();
}

void CreateModels(vec3 offset, vec3 p1, vec3 p2, vec3 p3)
{
    texture_coord = v_texture_coord[0];
    EmitPoint(p1, offset);

    texture_coord = v_texture_coord[1];
    EmitPoint(p2, offset);

    texture_coord = v_texture_coord[2];
    EmitPoint(p3, offset);

    EndPrimitive();
}

void main()
{
	// TODO(student): Emit the vertices of the triangle for which
	// information is received in the input attributes. Set the
	// following information for each vertex:
	//   - The coordinate of the vertex in clip space, transformed
	//     from world space, as received from the vertex shader
	//   - Texture coordinate received from the vertex shader.
	vec3 p1 = gl_in[0].gl_Position.xyz;
	vec3 p2 = gl_in[1].gl_Position.xyz;
	vec3 p3 = gl_in[2].gl_Position.xyz;

	const vec3 INSTANCE_OFFSET = vec3(1.25, 0, 1.25);

	vec3 offset = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < instances; i++)
    {
        offset = INSTANCE_OFFSET * vec3(5.0f * i, 0, 0.0f);
        CreateModels(offset, p1, p2, p3);
    }
}