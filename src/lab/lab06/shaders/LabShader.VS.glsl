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
    
    // aici prin inmultire are loc scalarea; puteam sa si pun operatia la final cred dar aici
    // mi s-a parut mai usor de construit aici
    // pulsatia are loc prin functia de sinus, astfel scara de modifica in mod limitat controlat si constant_id
    // adun 1 pt ca scara "de baza" sa fie 1, si asa variaza intre 0 si 2 defapt, insa inmultesc cu 0.1 pt vizualizare mai placuta
    vec3 pos = v_position * vec3(1 + 0.1 * sin(time));

    // float rand = fract(position.x * 124.32+ position.y * 3421.53 + position.z* 434.24); 

    // TODO(student): Compute gl_Position

    gl_Position = Projection * View * Model * vec4(v_position, 1);

}