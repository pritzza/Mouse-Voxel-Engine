#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in int aSurrounding;

out VertexData {
	highp int surrounding;
} outData;

void main()
{
	outData.surrounding = aSurrounding;
	gl_Position = vec4(aPos, 1.0);
}