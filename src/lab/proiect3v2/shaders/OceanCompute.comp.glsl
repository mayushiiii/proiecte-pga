#version 430 core

layout (local_size_x = 16, local_size_y = 16) in;

layout(rgba32f, binding = 0) writeonly uniform image2D displacementMap;
layout(rgba32f, binding = 1) writeonly uniform image2D normalMap;

uniform float time;
uniform int num_waves;

uniform vec2 wave_directions[16];
uniform float wave_amplitudes[16];
uniform float wave_lengths[16];

float g = 9.81; //gravitatia
float pi = 3.14;

void main()
{
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
    ivec2 img_size = imageSize(displacementMap);

	if(pixel_coords.x >= img_size.x || pixel_coords.y >= img_size.y) return;

	//conversie in -1 1
	float u = float(pixel_coords.x) / float(img_size.x - 1);
    float v = float(pixel_coords.y) / float(img_size.y - 1);
    vec3 initial_position = vec3(u * 2.0f - 1.0f, 0.0f, v * 2.0f - 1.0f);

	vec3 displacement = vec3(0.0, 0.0, 0.0);

	//normale
	vec3 tangent = vec3(1.0, 0.0, 0.0);
    vec3 bitangent = vec3(0.0, 0.0, 1.0);

	for (int i = 0; i < num_waves; i++) {
        vec2 d = wave_directions[i];
        float a = wave_amplitudes[i];
        float l = wave_lengths[i];

        float k = (2 * pi) / l;
        float omega = sqrt(g * k); //viteza valurilor
        float Q = 1 / (omega * a * num_waves); //steepness - cu cat e mai mic, cu atat sunt mai rotunjite valurile, cu cat e mai mare, cu atat sunt mai ascutite valurile

        // theta : k(D . xz) - w*t
        //float theta = k * dot(d, initial_position.xy) - (omega * 0.3 * time); //teoretic acelasi lucru cu linia urm, practic nu
        float theta = k * (d.x * initial_position.x + d.y * initial_position.z) - (omega * 0.3 * time);

        float cos_theta = cos(theta);
        float sin_theta = sin(theta);

        //displacement
        displacement.x += Q * a * d.x * cos_theta;
        displacement.y += a * sin_theta;
        displacement.z += Q * a * d.y * cos_theta;

        //normale
        //derivata partiala dupa x
        tangent.x = 1- Q * a * k * d.x * d.x * sin_theta;
        tangent.y += a * k * d.x * cos_theta;
        tangent.z -= Q * a * k * d.x * d.y * sin_theta;

        //derivata partiala dupa z
        bitangent.x -= Q * a * k * d.x * d.y * sin_theta;
        bitangent.y += a * k * d.y * cos_theta;
        bitangent.z = 1 - Q * a * k * d.y * d.y * sin_theta;
    }

    vec3 final_normal = normalize(cross(bitangent, tangent));

    imageStore(displacementMap, pixel_coords, vec4(displacement, 1));
    imageStore(normalMap, pixel_coords, vec4(final_normal, 1));
}