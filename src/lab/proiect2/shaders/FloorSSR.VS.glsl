#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 viewPosition;
out vec3 viewNormal;

void main()
{
    vec4 worldPosition = Model * vec4(v_position, 1.0);
    vec4 viewPos = View * worldPosition;

    viewPosition = viewPos.xyz;

    mat3 normalMatrix = mat3(transpose(inverse(View * Model)));
    viewNormal = normalize(normalMatrix * v_normal);

    gl_Position = Projection * viewPos;
}