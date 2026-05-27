#version 430 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout (binding = 0, rgba32f) writeonly uniform image2D initialSpectrumImage;

uniform int N; 
uniform float L;
uniform float amplitude;
uniform vec2 windDirection;
uniform float windSpeed;

const float PI = 3.14159265359;
const float g = 9.81;


uint hash(uint seed) 
{
    seed ^= seed >> 16;
    seed *= 0x7feb352dU;
    seed ^= seed >> 15;
    seed *= 0x846ca68bU;
    seed ^= seed >> 16;
    return seed;
}

float random(uint seed) 
{
    return float(hash(seed)) / 4294967295.0;
}

vec2 gaussianRandomVariable(uint seed) 
{
    float u1 = random(seed);
    float u2 = random(seed + 123456789U);
    
    u1 = clamp(u1, 0.0001, 1.0);
    
    float r = sqrt(-2.0 * log(u1));
    float theta = 2.0 * PI * u2;
    
    return vec2(r * cos(theta), r * sin(theta));
}

float calculateSpectrum(vec2 k) 
{ 
    float k_len = length(k);
    if (k_len < 0.0001) k_len = 0.0001;

    vec2 k_dir = normalize(k);
    float L_wind = (windSpeed * windSpeed) / g;

    float k_dot_w = dot(k_dir, normalize(windDirection));
    if (k_dot_w < 0.0) return 0.0;

    float k_len2 = pow(k_len, 2);
    float k_len4 = pow(k_len, 4);
    float damping = 0.001;

    float P = amplitude * exp(-1.0 / (k_len2 * L_wind * L_wind)) / k_len4 * pow(k_dot_w, 2.0);
    P *= exp(-k_len2 * L_wind * L_wind * damping * damping);
    
    float l = L_wind * 0.001;
    P *= exp(-k_len2 * l * l);
    
    return P;
}

void main() 
{
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

    // Mirror coordinates for -k
    ivec2 mirror_coords = ivec2((N - pixel_coords.x) % N, (N - pixel_coords.y) % N);

    float n = float(pixel_coords.x) - float(N) / 2.0;
    float m = float(pixel_coords.y) - float(N) / 2.0;
    vec2 k = vec2(2.0 * PI * n / L, 2.0 * PI * m / L);

    // Only one random sample per pixel — mirror pixel handles its own
    uint seed = uint(pixel_coords.x + pixel_coords.y * N) * 1234567U;
    vec2 xi_k = gaussianRandomVariable(seed);

    float spectrum_k = calculateSpectrum(k);
    vec2 h0_k = xi_k * sqrt(max(spectrum_k, 0.0) / 2.0);

    // For the conjugate term, use the mirror pixel's own random sample
    uint mirror_seed = uint(mirror_coords.x + mirror_coords.y * N) * 1234567U;
    vec2 xi_mirror = gaussianRandomVariable(mirror_seed);
    
    vec2 k_mirror = -k;
    float spectrum_mk = calculateSpectrum(k_mirror);
    
    vec2 h0_mk = xi_mirror * sqrt(max(spectrum_mk, 0.0) / 2.0);

    imageStore(initialSpectrumImage, pixel_coords, vec4(h0_k.x, h0_k.y, h0_mk.x, -h0_mk.y));
}