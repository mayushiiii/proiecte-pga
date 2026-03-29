#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output value to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec2 texture_coord;

void main()
{
    // TODO(student): Compute world space vertex position and normal,
    // and send them to the fragment shader

    texture_coord = v_texture_coord;
    world_position = (Model * vec4(v_position, 1.0)).xyz;
    world_normal = (Model * vec4(v_normal, 0.0)).xyz;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
