#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

out vec2 texture_coord;

void main()
{
    texture_coord = v_texture_coord;

    vec3 pos = v_position;
    pos.y += sin(time * 2.0) * 0.35 + 1;

    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}