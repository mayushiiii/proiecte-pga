#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0)in vec3 v_position;
layout(location = 1)in vec3 v_normal;
layout(location = 2)in vec2 v_tex_coord;
layout(location = 3)in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// Output
// TODO(student): Output values to fragment shader
out vec3 f_color;
out float f_time;

void main()
{
    // TODO(student): Send output to fragment shader
    f_color = v_color;
    f_time = time;

    // TODO(student): Compute gl_Position
    // oscilatia pe axa oy se face aici prin functia sinus. pozitia corpului se modifica in baza sinusoidei si urca si coboara constant astfel
    // prin adunarea sinusului se face miscarea
    gl_Position = Projection * View * (Model * vec4(v_position, 1.0) + vec4(0, sin(time), 0, 0));
}