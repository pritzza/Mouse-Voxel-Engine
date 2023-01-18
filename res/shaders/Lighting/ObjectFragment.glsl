#version 330 core

// taken from src/gfx/Lighting.h
// MAKE SURE THE STRUCTURES ARE IDENTICAL
struct Material
{
	vec3 ambientColor;  // color under ambient lighting
	vec3 diffuseColor;  // color under diffuse lighting (surface color)
	vec3 specularColor;	// color of specular highlight

	float shininess;    // scattering/radius of specular highlight
};

struct Light
{
	vec3 position;		// light source position

	vec3 ambientColor;	// intensity of ambient light in each RGB component
	vec3 diffuseColor;	// color of light source
	vec3 specularColor;	// generall left at vec3(1.0) for max shining
};

in vec3 voxelColor;

// for diffuse lighting
in vec3 normal;
in vec3 fragPosition;

out vec4 FragColor;

// for specular lighting
uniform vec3 viewPosition;

uniform Material material;
uniform Light light;

void main()
{
    // ambient lighting
    vec3 ambientColor = material.ambientColor * light.ambientColor;

    // diffuse lighting
    vec3 lightDirection = normalize(light.position - fragPosition);
    float diffuseStrength = max(dot(normal, lightDirection), 0);
    //vec3 diffuseColor = diffuseStrength * material.diffuseColor * light.diffuseColor;
    vec3 diffuseColor = 
        diffuseStrength * material.diffuseColor * light.diffuseColor * voxelColor;

    // specular lighting
    vec3 viewDirection = normalize(fragPosition - viewPosition);
    vec3 reflectionDirection = reflect(lightDirection, normal);

    float specular = pow( max( dot(viewDirection, reflectionDirection), 0), material.shininess);
    vec3 specularColor = specular * material.specularColor * light.specularColor;

    vec3 finalColor = ambientColor + diffuseColor + specularColor;

    FragColor = vec4(finalColor, 1.0);
} 