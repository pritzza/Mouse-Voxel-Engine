#version 330 core

in vec4 albedo;
in vec3 normal;
in float ao;
in vec3 fragCoord;

out vec4 FragColor;

uniform sampler2D depthBuffer;

uniform mat4 lightView;
uniform mat4 lightPerspective;

// todo: add ambient occlusion
// idea: CPU side, every voxel stores bitwise whether all the pixels
//		 surrounding it are transparent (0) or opaque (1).
//		 GPU takes this and can compute which faces of each voxel
//		 are visible, normals, and ambient occlusion from it

void main()
{
	ivec2 iResolution = ivec2(1920,1080);

	vec2 uv = (fragCoord.xy + 1.0) / 2.0; // from [-1,1] to [0,1]
	float depth = texture(depthBuffer, uv).r;

	vec4 lightColor = vec4(1.0);
	vec3 lightDir = normalize( vec3(1.0, 2.0, 1.5) );
	float MIN_LIGHT_STRENGTH = .2;
	float lightStrength = max(dot(lightDir, normal), MIN_LIGHT_STRENGTH);
	//lightStrength = (dot(lightDir, normal) + 1) / 2.0;
	
	vec4 garbage = (lightPerspective * lightView * vec4(1.0)) * 0.000001;

	FragColor = albedo * lightColor * lightStrength * ao;
	FragColor = vec4(vec3(depth), 1.0) + garbage;
}