#version 330 core

in vec4 albedo;
in vec3 normal;
in float ao;

out vec4 FragColor;

uniform float time;

// todo: add ambient occlusion
// idea: CPU side, every voxel stores bitwise whether all the pixels
//		 surrounding it are transparent (0) or opaque (1).
//		 GPU takes this and can compute which faces of each voxel
//		 are visible, normals, and ambient occlusion from it

void main()
{
	vec4 lightColor = vec4(1.0);
	vec3 lightDir = normalize( vec3(1.0, 2.0, 1.5) );
	float MIN_LIGHT_STRENGTH = .2;
	float lightStrength = max(dot(lightDir, normal), MIN_LIGHT_STRENGTH);
	lightStrength = (dot(lightDir, normal) + 1) / 2.0;
	
	FragColor = albedo * lightColor * lightStrength * (1-(ao/8)) + (time/100000);
	FragColor = albedo * lightColor * lightStrength * 1 + (time/100000);
}