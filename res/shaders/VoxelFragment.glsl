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

float LinearizeDepth(float depth)
{
    return depth;
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

float shadowCalculation(vec4 fragPosLightClipSpace)
{
    // Perform perspective divide for orthographic projection
    vec3 projCoords = fragPosLightClipSpace.xyz;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Check if the fragment is outside the light frustum
    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0)
    {
        return 0.0; // Fully lit
    }

    // Fetch the depth from the shadow map
    float closestDepth = texture(depthBuffer, projCoords.xy).r;
    closestDepth = LinearizeDepth(closestDepth);

    // Current depth of the fragment in light space
    float currentDepth = projCoords.z;

    // Apply a bias to reduce shadow acne
    float bias = max(0.005 * (1.0 - dot(normalize(normal), -lightDirection)), 0.005);

    // Check if the fragment is in shadow
    float shadow = currentDepth > closestDepth + bias ? 1.0 : 0.0;

    return shadow;
}



void main()
{
	vec2 uv = (fragPosCameraClipSpace.xy + 1.0) / 2.0; // from [-1,1] to [0,1]
	float closestDepth = LinearizeDepth(texture(depthBuffer, uv).r);

	float shadow = shadowCalculation(fragPosLightClipSpace);

	vec4 lightColor = vec4(1.0);
	vec3 lightDir = normalize( lightDirection );
	float MIN_LIGHT_STRENGTH = .2;
	float lightStrength = max((1-shadow) * dot(-lightDir, normal), MIN_LIGHT_STRENGTH);
	//lightStrength = (dot(lightDir, normal) + 1) / 2.0;
	
	vec4 garbage = (lightProjection * lightView * vec4(1.0)) * 0.000001 + vec4(lightDirection, 0.0) * 0.00001 + (0.00000001 * nearPlane + 0.00000001 * farPlane);
	
	vec4 normalShadingDebugging = vec4((normal + 1.0) / 2.0, 1.0);
	vec4 depthBufferDebugging = vec4(vec3(closestDepth), 1.0);
	vec4 regularColoring = ao * lightStrength * lightColor * albedo;

	FragColor = regularColoring + garbage/1000000;
}