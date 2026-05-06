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
    vec4 clipSpacePosition = vec4(uv * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);

    vec4 viewSpacePosition = inverseProjectionMatrix * clipSpacePosition;
    viewSpacePosition /= viewSpacePosition.w;

    return viewSpacePosition.xyz;
}

vec2 BinarySearchScreenSpace(vec3 lastVisiblePos, vec3 firstHitPos)
{
    vec3 startRelative = lastVisiblePos;
    vec3 endRelative = firstHitPos;
    vec3 mid;

    for(int i = 0; i < 10; i++)
    {
        mid = mix(startRelative, endRelative, 0.5);
        vec4 clipSpace = projectionMatrix * vec4(mid, 1.0);
        vec2 uv_bs = (clipSpace.xy / clipSpace.w) * 0.5 + 0.5;

        float sceneDepthBS = texture(depthTexture, uv_bs).r;
        vec3 scenePos = ReconstructViewPosition(uv_bs, sceneDepthBS);

        if(mid.z < scenePos.z) 
        {
            endRelative = mid;
        }
        else
        {
            startRelative = mid;
        }
    }

    vec4 finalClip = projectionMatrix * vec4(mid, 1.0);
    return (finalClip.xy / finalClip.w) * 0.5 + 0.5;
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

   while(traveled < maxDistance)
    {
        rayPosition += reflectionDirection * viewSpaceStepSize;
        traveled += viewSpaceStepSize;

        vec4 clipSpacePosition = projectionMatrix * vec4(rayPosition, 1.0);

        if (clipSpacePosition.w <= 0.0) 
            break;

        vec3 screenSpacePosition = (clipSpacePosition.xyz / clipSpacePosition.w) * 0.5 + 0.5;

        vec2 uv = screenSpacePosition.xy;

        if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) 
            break;

        float sceneDepth = texture(depthTexture, uv).r;

        if (sceneDepth >= 0.999) 
            continue;

        vec3 sceneViewPosition = ReconstructViewPosition(uv, sceneDepth);

        float difference = rayPosition.z - sceneViewPosition.z;

        if (difference > 0.0 && difference < thickness)
        {
            vec3 lastVisiblePosition = rayPosition - (reflectionDirection * viewSpaceStepSize);
            vec2 bs_result = BinarySearchScreenSpace(lastVisiblePosition, rayPosition);
            return texture(colorTexture, bs_result).rgb;
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