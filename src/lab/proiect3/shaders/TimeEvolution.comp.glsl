#version 430 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout (binding = 0, rgba32f) readonly uniform image2D initialSpectrumImage;

layout (binding = 1, rgba32f) writeonly uniform image2D spectrumY; // Height displacement
layout (binding = 2, rgba32f) writeonly uniform image2D spectrumX; // Choppiness (Horizontal X)
layout (binding = 3, rgba32f) writeonly uniform image2D spectrumZ; // Choppiness (Horizontal Z)

uniform int N;
uniform float L;
uniform float time;

const float PI = 3.14159265359;
const float g = 9.81;

vec2 complexMultiply(vec2 c1, vec2 c2) 
{
    return vec2(c1.x * c2.x - c1.y * c2.y, c1.x * c2.y + c1.y * c2.x);
}

void main() 
{
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

    float n = float(pixel_coords.x) - float(N) / 2.0;
    float m = float(pixel_coords.y) - float(N) / 2.0;

    vec2 k = vec2(2.0 * PI * n / L, 2.0 * PI * m / L);
    float k_len = length(k);
    if (k_len < 0.0001) k_len = 0.0001;

    float omega = sqrt(g * k_len);

    vec4 h0_data = imageLoad(initialSpectrumImage, pixel_coords);

    vec2 h0 = h0_data.xy;
    vec2 h0_conj = h0_data.zw;

    float phase = omega * time;
    vec2 cos_sin = vec2(cos(phase), sin(phase));
    vec2 cos_minus_sin = vec2(cos_sin.x, -cos_sin.y);

    vec2 h_k_t = complexMultiply(h0, cos_sin) + complexMultiply(h0_conj, cos_minus_sin);
    vec2 k_dir = k / k_len;

    vec2 h_k_t_dx = vec2(-h_k_t.y * k_dir.x, h_k_t.x * k_dir.x); 
    vec2 h_k_t_dz = vec2(-h_k_t.y * k_dir.y, h_k_t.x * k_dir.y);
    
    imageStore(spectrumY, pixel_coords, vec4(h_k_t, 0.0, 0.0));
    imageStore(spectrumX, pixel_coords, vec4(h_k_t_dx, 0.0, 0.0));
    imageStore(spectrumZ, pixel_coords, vec4(h_k_t_dz, 0.0, 0.0));
}