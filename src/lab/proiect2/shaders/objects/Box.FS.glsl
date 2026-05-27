#version 330

in vec2 texture_coord;

uniform sampler2D texture_1;
uniform float time;

layout(location = 0) out vec4 out_color;

void main()
{
    vec2 out_texture_coord = texture_coord;

    out_color = texture(texture_1, out_texture_coord);

    if (out_color.a < 0.5)
        discard;
}