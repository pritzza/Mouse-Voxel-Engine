#version 330 core

in vec4 albedo;
in vec3 normal;
in float ao;
in vec4 fragPosCameraClipSpace;
in vec4 fragPosLightClipSpace;

out vec4 FragColor;

uniform sampler2D depthBuffer;

uniform mat4 lightView;
uniform mat4 lightProjection;

uniform vec3 lightDirection;

uniform float nearPlane;
uniform float farPlane;

float shadowCalculation(vec4 fragPosLightClipSpace)
{
    // need perpective divide when doing perspective projection
    vec3 projCoords = fragPosLightClipSpace.xyz;

    // Transform from [-1,1] range to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // If outside the [0,1] range, the fragment is outside the shadow map
    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z < 0.0 || projCoords.z > 1.0)
    {
        return 0.0; 
    }

    // Read depth from the orthographic shadow map (already in [0,1] range)
    float closestDepth = texture(depthBuffer, projCoords.xy).r;

    // Current depth also in [0,1]
    float currentDepth = projCoords.z;

    // Apply a small bias to reduce shadow acne
    float bias = 0.001;

    // Compare current depth against stored depth
    float shadow = 0.0;

    vec2 texelSize = 1.0 / textureSize(depthBuffer, 0);

    for (int x = -1; x <= 1; ++x)
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthBuffer, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    

    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}


void main()
{
    // debugging shadow mapping
	//vec2 uv = (fragPosCameraClipSpace.xy + 1.0) / 2.0; // from [-1,1] to [0,1]
	//float closestDepth = texture(depthBuffer, uv).r;
	//vec4 depthBufferDebugging = vec4(vec3(closestDepth), 1.0);

    // debugging
	vec4 normalShadingDebugging = vec4((normal + 1.0) / 2.0, 1.0);
    
    
    // actual lighting calculations

	vec3 lightColor = vec3(1.0);
	
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize( lightDirection );
    float diffuseStrength = max(dot(-lightDir, normal), 0.0) * (1.0 - ambientStrength);
    vec3 diffuse = vec3(diffuseStrength) * lightColor;

	float shadow = shadowCalculation(fragPosLightClipSpace);

	vec3 regularColoring = (ao * ambient + (1.0 - shadow) * diffuse) * vec3(albedo);

	FragColor = vec4(regularColoring, 1.0);
}