#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 30) out;

in VertexData {
	vec4 color;
	int faces;
} inData[];	// array of all vertices in primitive (only one cuz point)

// for camera transform
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

uniform vec3 viewPosition;

out vec4 albedo;
out vec3 normal;

bool backFaceCull = true;

bool shouldCull(const vec3 normal, const vec3 faceOffset)
{
	vec3 voxelPos = vec3(gl_in[0].gl_Position);
	
	vec3 facePos = vec3(model * vec4(voxelPos + faceOffset, 1.0));
	vec3 viewDir = normalize(viewPosition - facePos);

	return dot(viewDir, normal) <= 0;
}

vec4 cameraTransform(const vec3 pos)
{
	return perspective * view * model * vec4(pos, 1.0);
}

void makeTopFace(const vec3 p)
{
	const vec3[4] vertices = vec3[4](
		vec3(0.0, 1.0, 0.0),
		vec3(1.0, 1.0, 0.0),
		vec3(0.0, 1.0, 1.0),
		vec3(1.0, 1.0, 1.0)
	);

	normal = vec3(0.0, 1.0, 0.0);

	if (shouldCull(normal, vertices[0]))
		return;

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}
	
	EndPrimitive();
}

void makeBottomFace(const vec3 p)
{
	const vec3[4] vertices = vec3[4](
		vec3(0.0, 0.0, 0.0),
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 0.0, 1.0),
		vec3(1.0, 0.0, 1.0)
	);

	normal = vec3(0.0, -1.0, 0.0);
	
	if (shouldCull(normal, vertices[0]))
		return;

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}

	EndPrimitive();
}

void makeFrontFace(const vec3 p)
{
	const vec3[4] vertices = vec3[4](
		vec3(0.0, 0.0, 1.0),
		vec3(1.0, 0.0, 1.0),
		vec3(0.0, 1.0, 1.0),
		vec3(1.0, 1.0, 1.0)
	);

	normal = vec3(0.0, 0.0, 1.0);
	
	if (shouldCull(normal, vertices[0]))
		return;

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}

	EndPrimitive();
}

void makeBackFace(const vec3 p)
{
	const vec3[4] vertices = vec3[4](
		vec3(0.0, 0.0, 0.0),
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0),
		vec3(1.0, 1.0, 0.0)
	);

	normal = vec3(0.0, 0.0, -1.0);
	
	if (shouldCull(normal, vertices[0]))
		return;

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}

	EndPrimitive();
}

void makeLeftFace(const vec3 p)
{
	const vec3[4] vertices = vec3[4](
		vec3(0.0, 0.0, 0.0),
		vec3(0.0, 0.0, 1.0),
		vec3(0.0, 1.0, 0.0),
		vec3(0.0, 1.0, 1.0)
	);

	normal = vec3(-1.0, 0.0, 0.0);
	
	if (shouldCull(normal, vertices[0]))
		return;

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}

	EndPrimitive();
}

void makeRightFace(const vec3 p)
{
	const vec3[4] vertices = vec3[4](
		vec3(1.0, 0.0, 0.0),
		vec3(1.0, 0.0, 1.0),
		vec3(1.0, 1.0, 0.0),
		vec3(1.0, 1.0, 1.0)
	);

	normal = vec3(1.0, 0.0, 0.0);
	
	if (shouldCull(normal, vertices[0]))
		return;

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}

	EndPrimitive();
}

void makeVoxel()
{
	vec3 pos = vec3(gl_in[0].gl_Position);
	int faces = inData[0].faces;

	albedo = inData[0].color;

	bool addTop		= bool((1 << 0) & faces);
	bool addBottom	= bool((1 << 1) & faces);
	bool addFront	= bool((1 << 2) & faces);
	bool addBack	= bool((1 << 3) & faces);
	bool addLeft	= bool((1 << 4) & faces);
	bool addRight	= bool((1 << 5) & faces);

	if (addTop)
		makeTopFace(pos);
	if (addBottom)
		makeBottomFace(pos);
	if (addFront)
		makeFrontFace(pos);
	if (addBack)
		makeBackFace(pos);
	if (addLeft)
		makeLeftFace(pos);
	if (addRight)
		makeRightFace(pos);
}

void main()
{
	makeVoxel();
}