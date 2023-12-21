#version 330 core

in vec4 albedo;
in vec3 normal;

out vec4 FragColor;

void main()
{
	vec4 lightColor = vec4(1.0);
	vec3 lightDir = normalize( vec3(1.0, 2.0, 0.0) );
	float lightStrength = (dot(lightDir, normal) + 1) / 2.0;
	
	FragColor = albedo * lightColor * lightStrength;
}