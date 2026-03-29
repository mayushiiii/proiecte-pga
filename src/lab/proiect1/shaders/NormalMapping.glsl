#version 330

in vec3 world_position;
in vec3 world_normal;
in vec2 texture_coord;

// must come from vertex shader
in mat3 TBN;

uniform sampler2D texture_1;   // diffuse
uniform sampler2D normalMap;   // normal map

uniform int point_lights_count;
uniform vec3 point_light_positions[11];
uniform vec3 point_light_colors[11];

uniform vec3 eye_position;
uniform vec3 material_ka;
uniform vec3 material_kd;
uniform vec3 material_ks;
uniform int material_shininess;

layout(location = 0) out vec4 out_color;


vec3 ComputePhongIllumination(vec3 light_position)
{
    vec3 normal_tex = texture(normalMap, texture_coord).rgb;
    normal_tex = normal_tex * 2.0 - 1.0;   
    // aici adaug TBN
    normal_tex *= TBN ; 
    vec3 N = normalize(normal_tex); 
    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 R = reflect(-L, N);

    vec3 diffuse_component = material_kd * max(dot(N, L), 0.0);

    vec3 specular_component = vec3(0.0);
    if (length(diffuse_component) > 0.0)
    {
        specular_component = material_ks *
            pow(max(dot(R, V), 0.0), float(material_shininess));
    }

    return diffuse_component + specular_component;
}


float ComputeDistanceAttenuation(vec3 light_position, vec3 point_position)
{
    return 1.0 / (distance(light_position, point_position) + 1.0);
}


vec3 ComputePointLightSourcesIllumination()
{
    vec3 lights_illumination = vec3(0.0);

    for (int i = 0; i < point_lights_count; i++) {
        vec3 light_position = point_light_positions[i];
        vec3 light_color = point_light_colors[i];

        vec3 light_illumination = ComputePhongIllumination(light_position);
        float att = ComputeDistanceAttenuation(light_position, world_position);

        lights_illumination += light_color * light_illumination * att;
    }

    return lights_illumination;
}


vec3 ComputeAmbientComponent()
{
    vec3 global_ambient_color = vec3(0.25);
    return material_ka * global_ambient_color;
}


void main()
{
    vec4 tex_color = texture(texture_1, texture_coord);

    vec3 illumination =
        ComputePointLightSourcesIllumination() +
        ComputeAmbientComponent();

    vec3 final_rgb = illumination * tex_color.rgb;

    out_color = vec4(final_rgb, tex_color.a);
}