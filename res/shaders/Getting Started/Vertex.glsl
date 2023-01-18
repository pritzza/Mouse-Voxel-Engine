#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 textureCoordinate;

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
	// unused now
	float timeOffset = sin(time) / 2;

	gl_Position = perspective * view * model * vec4(aPos, 1.0);

	ourColor = aColor;
	textureCoordinate = aTexCoord;
}