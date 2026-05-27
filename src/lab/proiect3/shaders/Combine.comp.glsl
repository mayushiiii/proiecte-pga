#version 430 core
layout (local_size_x = 16, local_size_y = 16) in;

layout (binding = 0, rgba32f) readonly uniform image2D spaceX;
layout (binding = 1, rgba32f) readonly uniform image2D spaceY;
layout (binding = 2, rgba32f) readonly uniform image2D spaceZ;
layout (binding = 3, rgba32f) writeonly uniform image2D displacementMap;

uniform int N;
uniform float choppiness; 

void main() 
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    if (id.x >= N || id.y >= N) return;

    float dx = imageLoad(spaceX, id).x;
    float dy = imageLoad(spaceY, id).x;
    float dz = imageLoad(spaceZ, id).x;

    float per_element_scale = 1 / float(N * N);
    dx *= per_element_scale;
    dy *= per_element_scale;
    dz *= per_element_scale;

    if ((id.x + id.y) % 2 != 0) 
    {
        dx = -dx;
        dy = -dy;
        dz = -dz;
    }

    dx *= choppiness;
    dz *= choppiness;

    imageStore(displacementMap, id, vec4(dx, dy, dz, 1.0));
    
}