#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 4) in int aFaces;

out VertexData {
	vec4 color;
	int faces;
} outData;

void main()
{
	outData.color = aColor;
	outData.faces = aFaces;
	gl_Position = vec4(aPos, 1.0);
}