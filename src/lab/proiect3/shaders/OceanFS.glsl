#version 430

// Input
// TODO(student): Get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;
in vec2 texture_coord;

uniform sampler2D texture_1;
// Uniforms for light properties
uniform int point_lights_count;
uniform vec3 point_light_positions[11];
uniform vec3 point_light_colors[11];
uniform vec3 spot_light_positions[10];
uniform vec3 spot_light_colors[10];
uniform vec3 spot_light_directions[10];
uniform float spot_light_angles[10];
uniform vec3 eye_position;
uniform vec3 material_ka;
uniform vec3 material_kd;
uniform vec3 material_ks;
uniform int material_shininess;

// Output
layout(location = 0) out vec4 out_color;

vec3 ComputePhongIllumination(vec3 light_position)
{
    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 R = reflect(-L, N);

    // TODO(student): Compute the diffuse component of the Lambert illumination model
    vec3 diffuse_component = material_kd * max(dot(N, L), 0.0);

    // TODO(student): Compute the specular component of the Phong illumination model
    vec3 specular_component = vec3(0.0);

    if (length(diffuse_component) > 0.0)
    {
        specular_component = material_ks * pow(max(dot(R, V), 0.0), float(material_shininess));
    }

    // TODO(student): Compute the final illumination as the sum of the diffuse and specular components
    vec3 illumination = diffuse_component + specular_component;

    return illumination;
}

float ComputeDistanceAttenuation(vec3 light_position, vec3 point_position)
{
    // TODO(student): Compute the light attenuation factor based on the distance
    // between the position of the illuminated point and the position of the light source.
    return 1.0 / (distance(light_position, point_position) + 1.0);
}

vec3 ComputePointLightSourcesIllumination()
{
    vec3 lights_illumination = vec3(0.0);

    for (int i = 0; i < point_lights_count; i++) {
        vec3 light_position = point_light_positions[i];
        vec3 light_color = point_light_colors[i];

        vec3 light_illumination = ComputePhongIllumination(light_position);
        float illumination_attenuation = ComputeDistanceAttenuation(light_position, world_position);

        // TODO(student): Add to the illumination of all light sources the result
        // of multiplying the illumination of the light source from the current iteration
        // with the attenuation of the illumination and the color of the illumination.
        lights_illumination += light_color * light_illumination * illumination_attenuation;
    }

    return lights_illumination;
}

vec3 ComputeSpotLightSourcesIllumination()
{
    vec3 lights_illumination = vec3(0.0);

    for (int i = 0; i < 10; i++) {
        vec3 light_position = spot_light_positions[i];
        vec3 light_color = spot_light_colors[i];
        vec3 light_direction = normalize(spot_light_directions[i]);

        float phi_angle = spot_light_angles[i];
        vec3 L = normalize(light_position - world_position);

        float cos_theta_angle = dot(-L, light_direction);
        float cos_phi_angle = cos(phi_angle);

        if (cos_theta_angle > cos_phi_angle)
        {
            vec3 light_illumination = ComputePhongIllumination(light_position);
            float illumination_attenuation = ComputeDistanceAttenuation(light_position, world_position);

            // TODO(student): Compute the attenuation factor specific to the spot light source
            float spot_linear_att_factor = (cos_theta_angle - cos_phi_angle) / (1.0 - cos_phi_angle);
            float quadratic_spot_light_att_factor = spot_linear_att_factor * spot_linear_att_factor;

            // TODO(student): Add to the illumination of all light sources the result
            // of multiplying the illumination of the light source from the current iteration
            // with the attenuation of the illumination, the attenuation factor specific
            // to the light spot source and the color of the illumination.
            lights_illumination += light_color * light_illumination * illumination_attenuation * quadratic_spot_light_att_factor;
        }
    }

    return lights_illumination;
}

vec3 ComputeAmbientComponent()
{
    vec3 global_ambient_color = vec3(0.25);

    // TODO(student): Compute the ambient component of global illumination
    vec3 ambient_component = material_ka * global_ambient_color;

    return ambient_component;
}

void main()
{
    vec3 N = normalize(world_normal);
    vec3 V = normalize(eye_position - world_position);

    // Lumina principală: folosește becul alb controlabil, index 9
    vec3 light_position = point_light_positions[9];
    vec3 L = normalize(light_position - world_position);
    vec3 R = reflect(-L, N);

    float diff = max(dot(N, L), 0.0);

    // Specular mai îngust, mai "wet", mai puțin plastic
    float spec = pow(max(dot(R, V), 0.0), 120.0);

    // Fresnel: apa devine mai luminoasă la unghiuri joase
    float fresnel = pow(1.0 - max(dot(N, V), 0.0), 5.0);

    // Culori mai oceanice, nu albastru pastel plastic
    vec3 deepWater = vec3(0.025, 0.120, 0.200);
    vec3 shallowWater = vec3(0.075, 0.370, 0.480);
    vec3 skyReflection = vec3(0.63, 0.80, 0.90);

    // Variație după orientarea normalei
    float facingUp = clamp(N.y, 0.0, 1.0);
    vec3 waterColor = mix(deepWater, shallowWater, facingUp);

    // Reflexie de cer prin Fresnel
    vec3 color = mix(waterColor, skyReflection, fresnel * 0.65);

    // Iluminare simplă
    color *= 0.18 + diff * 0.45;

    // Highlight alb, nu albastru saturat
    color += vec3(1.0) * spec * 0.45;

    out_color = vec4(color, 1.0);
}