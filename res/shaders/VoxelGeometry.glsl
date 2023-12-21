#version 330 core

// attribute positions based on vertex attribute classes in src/gfx/VertexAttributes.h
layout (points) in;
layout (triangle_strip, max_vertices = 20) out;

in VS_OUT {
	float nothing;
} gs_in[];	// array of all vertices in primitive (only one cuz point)

// for camera transform
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

vec4 cameraTransform(const vec4 pos)
{
	return perspective * view * model * pos;
}

void makeCube()
{
	vec4 pos = gl_in[0].gl_Position;
}

void makeOppositeFaces()
{
	vec4 pos = gl_in[0].gl_Position;

	gl_Position = cameraTransform(pos + vec4(0.0, 0.0, 0.0, 0.0));
	EmitVertex();
	gl_Position = cameraTransform(pos + vec4(1.0, 0.0, 0.0, 0.0));
	EmitVertex();
	gl_Position = cameraTransform(pos + vec4(0.0, 1.0, 0.0, 0.0));
	EmitVertex();
	gl_Position = cameraTransform(pos + vec4(1.0, 1.0, 0.0, 0.0));
	EmitVertex();

	gl_Position = cameraTransform(pos + vec4(1.0, 1.0, 0.0, 0.0));
	EmitVertex();
	gl_Position = cameraTransform(pos + vec4(1.0, 1.0, 0.0, 0.0));
	EmitVertex();
	gl_Position = cameraTransform(pos + vec4(0.0, 0.0, 1.0, 0.0));
	EmitVertex();											
	gl_Position = cameraTransform(pos + vec4(0.0, 0.0, 1.0, 0.0));
	EmitVertex();	
	gl_Position = cameraTransform(pos + vec4(0.0, 0.0, 1.0, 0.0));
	EmitVertex();	
															
	gl_Position = cameraTransform(pos + vec4(0.0, 0.0, 1.0, 0.0));
	EmitVertex();											
	gl_Position = cameraTransform(pos + vec4(1.0, 0.0, 1.0, 0.0));
	EmitVertex();											
	gl_Position = cameraTransform(pos + vec4(0.0, 1.0, 1.0, 0.0));
	EmitVertex();											
	gl_Position = cameraTransform(pos + vec4(1.0, 1.0, 1.0, 0.0));
	EmitVertex();

	EndPrimitive();
}

void makeFace()
{
	vec4 pos = gl_in[0].gl_Position;

	gl_Position = cameraTransform(pos + vec4(0.0, 0.0, 0.0, 0.0));
	EmitVertex();
	gl_Position = cameraTransform(pos + vec4(1.0, 0.0, 0.0, 0.0));
	EmitVertex();
	gl_Position = cameraTransform(pos + vec4(0.0, 1.0, 0.0, 0.0));
	EmitVertex();
	gl_Position = cameraTransform(pos + vec4(1.0, 1.0, 0.0, 0.0));
	EmitVertex();

	EndPrimitive();
}

void main()
{
	makeOppositeFaces();
	makeCube();
	makeFace();
}