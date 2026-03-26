#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 f_color;
in float f_time;

// Output
layout(location = 0) out vec4 out_color;

// Calculate color
vec3 calculate_color(vec3 color)
{
	return vec3(color.x * cos(f_time), color.y, color.z);
}

void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(calculate_color(f_color), 1);

}

