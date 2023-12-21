#version 330 core

layout (location = 0) in vec3 aPos;

out VS_OUT {
	float nothing;
} vs_out;

void main()
{
	//vs_out.voxelPos = aPos;
	gl_Position = vec4(aPos, 1.0);
}