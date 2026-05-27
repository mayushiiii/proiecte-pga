#version 430 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout (binding = 0, rgba32f) readonly  uniform image2D sourceImage;
layout (binding = 1, rgba32f) writeonly uniform image2D destImage;

uniform int N;
uniform int stage;
uniform int direction;  

const float PI = 3.14159265359;

vec2 complexMultiply(vec2 a, vec2 b) 
{
    return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

void main() 
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    if (id.x >= N || id.y >= N) return;

    int coordinate  = (direction == 0) ? id.x : id.y;
    int other_coord = (direction == 0) ? id.y : id.x;

    int Ns = 1 << stage;          // sub-problem size at this stage

    int k  = coordinate % Ns;     // index within the sub-group
    int r  = coordinate / Ns;     // which sub-group

    // Stockham: one element from top half, one from bottom half
    int s0 = r * Ns + k;
    int s1 = s0 + N / 2;

    ivec2 src0 = (direction == 0) ? ivec2(s0, other_coord) : ivec2(other_coord, s0);
    ivec2 src1 = (direction == 0) ? ivec2(s1, other_coord) : ivec2(other_coord, s1);

    vec2 v0 = imageLoad(sourceImage, src0).xy;
    vec2 v1 = imageLoad(sourceImage, src1).xy;

    // IFFT uses +2*PI (forward FFT uses -2*PI)
    // changes wave direction
    float angle = 2.0 * PI * float(k) / float(Ns * 2);

    vec2 twiddle = vec2(cos(angle), sin(angle));

    vec2 even = v0 + complexMultiply(twiddle, v1);
    vec2 odd  = v0 - complexMultiply(twiddle, v1);

    // Stockham output: write to non-scrambled positions
    ivec2 dst_even = (direction == 0) ? ivec2(r * Ns * 2 + k, other_coord) : ivec2(other_coord, r * Ns * 2 + k);
    ivec2 dst_odd  = (direction == 0) ? ivec2(r * Ns * 2 + k + Ns, other_coord) : ivec2(other_coord, r * Ns * 2 + k + Ns);

    imageStore(destImage, dst_even, vec4(even, 0.0, 0.0));
    imageStore(destImage, dst_odd,  vec4(odd,  0.0, 0.0));
}