#version 330 core

// attribute positions based on vertex attribute classes in src/gfx/VertexAttributes.h
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 3) in vec3 aNormal;

out vec3 voxelColor;

// for diffuse lighting
out vec3 normal;
out vec3 fragPosition;

// for camera transform
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
	// for diffuse lighting
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	normal = normalize(normalMatrix * aNormal);

	fragPosition = vec3(model * vec4(aPos, 1.0));

	voxelColor = aColor;

	gl_Position = perspective * view * model * vec4(aPos, 1.0);
}