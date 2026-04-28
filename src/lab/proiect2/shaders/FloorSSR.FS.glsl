#version 330

in vec3 viewPosition;
in vec3 viewNormal;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform mat4 projectionMatrix;
uniform mat4 inverseProjectionMatrix;

layout(location = 0) out vec4 out_color;

vec3 ReconstructViewPosition(vec2 uv, float depth)
{
    vec4 clipSpacePosition = vec4(
        uv * 2.0 - 1.0,
        depth * 2.0 - 1.0,
        1.0
    );

    vec4 viewSpacePosition = inverseProjectionMatrix * clipSpacePosition;
    viewSpacePosition /= viewSpacePosition.w;

    return viewSpacePosition.xyz;
}

vec3 SSR(vec3 viewPosition, vec3 viewNormal)
{
    //implementare in view space
    vec3 viewDirection = normalize(viewPosition);
    vec3 reflectionDirection = reflect(viewDirection, normalize(viewNormal));

    float viewSpaceStepSize = 0.05;
    float maxDistance = 8.0;
    //marja de eroare
    float thickness = 0.08;

    float traveled = 0.0;
    vec3 rayPosition = viewPosition;

    while (traveled < maxDistance)
    {
        rayPosition += reflectionDirection * viewSpaceStepSize;
        traveled += viewSpaceStepSize;

        vec4 clipSpacePosition = projectionMatrix * vec4(rayPosition, 1.0);

        if (clipSpacePosition.w <= 0.0)
            break;

        vec3 screenSpacePosition = clipSpacePosition.xyz / clipSpacePosition.w;
        screenSpacePosition = screenSpacePosition * 0.5 + 0.5;

        vec2 uv = screenSpacePosition.xy;

        if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
            break;

        float sceneDepth = texture(depthTexture, uv).r;

        if (sceneDepth >= 0.999)
            continue;

        vec3 sceneViewPosition = ReconstructViewPosition(uv, sceneDepth);


        //compararea directa a valorilor de depth ducea la reflexii multiple (artefacte)
        //reconstruim pozitia in view space si comparam cu rayPosition ca sa ne asiguram ca raza intersecteaza corect geometria o singura data

        float difference = rayPosition.z - sceneViewPosition.z;

        if (difference > 0.0 && difference < thickness)
        {
            return texture(colorTexture, uv).rgb;
        }
    }

    return vec3(0.0);
}

void main()
{
    vec3 floorColor = vec3(0.8);
    vec3 reflectionColor = SSR(viewPosition, normalize(viewNormal));

    out_color = vec4(floorColor + reflectionColor * 0.35, 1.0);
}