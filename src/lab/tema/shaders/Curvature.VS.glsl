#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

float curvatureFactor = 0.01;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 eye_position;

// Output value to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec2 texture_coord;
out vec3 color;

void main()
{
    // TODO(student): Compute world space vertex position and normal,
    // and send them to the fragment shader
    world_position = (Model * vec4(v_position, 1.0)).xyz;
    world_normal = (Model * vec4(v_normal, 0.0)).xyz;

    // ok deci avem de folosit urmatoarea formula
    // P(v)=P(v)− control * ||P(cam)−P(v)||^2 * factorCurbura 
    // unde p(v) e pozitia vertexului, p(cam) e pozitia camerei, 
    // iar control e un vector care arata spre camera
    // altfel spus, e normalizat vectorul dintre pozitia camerei si pozitia vertexului
    vec3 control = vec3(0, 1, 0);
    float distance_squared = pow(distance(eye_position, world_position), 2.0);
    world_position = world_position - control * distance_squared * curvatureFactor;
    
    color = v_color;
    texture_coord = v_texture_coord;
    gl_Position = Projection * View  * vec4(world_position, 1.0);
}
