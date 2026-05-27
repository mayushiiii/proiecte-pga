#version 430

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// The new texture uniform you bound in C++
uniform sampler2D displacement_map;

// Output value to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec2 texture_coord; // Added this since your fragment shader expects it

void main()
{
    texture_coord = v_texture_coord;

    // ---- Displace the vertex ----
    vec4 spectrum = texture(displacement_map, v_texture_coord);
    vec3 displacement = spectrum.xyz * 0.2;
    vec3 displaced_position = v_position + displacement;
    world_position = (Model * vec4(displaced_position, 1.0)).xyz;

    // ---- Compute normal from height-map gradient (finite differences) ----
    float texelSize = 1.0 / 256.0;
    float h_L = texture(displacement_map, v_texture_coord + vec2(-texelSize, 0.0)).y;
    float h_R = texture(displacement_map, v_texture_coord + vec2( texelSize, 0.0)).y;
    float h_D = texture(displacement_map, v_texture_coord + vec2( 0.0, -texelSize)).y;
    float h_U = texture(displacement_map, v_texture_coord + vec2( 0.0,  texelSize)).y;

    // Smaller = sharper-looking waves, larger = flatter. Tune to taste.
    const float normalStrength = 0.02;
    vec3 normal = normalize(vec3(h_L - h_R, normalStrength, h_D - h_U));

    world_normal = normalize((Model * vec4(normal, 0.0)).xyz);

    gl_Position = Projection * View * vec4(world_position, 1.0);
}