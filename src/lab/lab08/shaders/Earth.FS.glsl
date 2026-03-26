#version 330

// Input
in vec2 texture_coord;
in float frag_time;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec2 out_texture_coord = texture_coord;
    out_texture_coord.x = out_texture_coord.x + sin(frag_time);
    out_color = texture2D(texture_1, out_texture_coord);

    // TODO(student): Use the "discard" directive to terminate execution
    // based on the value of the alpha channel
    if (out_color.a < 0.5)
		discard;
}
