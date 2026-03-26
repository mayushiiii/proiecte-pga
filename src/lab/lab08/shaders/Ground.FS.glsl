#version 330

in vec2 texture_coord;

uniform sampler2D texture_1;
uniform sampler2D texture_2;

layout(location = 0) out vec4 out_color;


void main()
{
    vec3 color = texture(texture_1, texture_coord).rgb;
    float height = texture(texture_2, texture_coord).r;

    color = mix(color, vec3(height, height, height), 1 - height);

    out_color = vec4(color, 1.0f);
}
