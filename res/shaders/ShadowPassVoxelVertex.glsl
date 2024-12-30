#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in int aSurrounding;

out VertexData {
	vec4 color;
	highp int surrounding;
} outData;

void main()
{
	outData.color = aColor;
	outData.surrounding = aSurrounding;
	gl_Position = vec4(aPos, 1.0);
}