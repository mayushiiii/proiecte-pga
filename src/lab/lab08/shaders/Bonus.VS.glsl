#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform sampler2D texture_2;
uniform float scale_factor;

out vec2 texture_coord;

void main()
{
    texture_coord = v_texture_coord;

    float height = texture(texture_2, v_texture_coord).r * scale_factor;

    gl_Position = Projection * View * (Model * vec4(v_position, 1.0) + vec4(0.0, height, 0.0, 0.0));
}
