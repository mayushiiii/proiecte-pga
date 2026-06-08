#version 430

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform sampler2D displacement_map;
uniform sampler2D normal_map; 

out vec3 world_position;
out vec3 world_normal;
out vec2 texture_coord;

void main()
{
    texture_coord = v_texture_coord;

    vec3 displacement = texture(displacement_map, v_texture_coord).xyz;
    
    vec3 displaced_position = v_position + displacement;
    world_position = (Model * vec4(displaced_position, 1.0)).xyz;

    vec3 calculated_normal = texture(normal_map, v_texture_coord).xyz;

    world_normal = normalize(mat3(Model) * calculated_normal);

    gl_Position = Projection * View * vec4(world_position, 1.0);
}