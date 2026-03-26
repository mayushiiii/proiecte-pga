#version 330

in vec2 texture_coord;

uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;
uniform sampler2D texture_4;

layout(location = 0) out vec4 out_color;

void main()
{
    vec3 color = texture(texture_1, texture_coord).rgb;
    float height = texture(texture_2, texture_coord).r;
    vec3 snow = texture(texture_3, texture_coord).rgb;
    vec3 water = texture(texture_4, texture_coord).rgb;

    color = mix(color, vec3(height, height, height), 1 - height);

    float water_blend = smoothstep(0.2, 0.3, height);
    float snow_blend = smoothstep(0.7, 0.8, height);

    color = mix(color, water, 1.0 - water_blend);
    color = mix(color, snow, snow_blend);

    out_color = vec4(color, 1.0);
}
