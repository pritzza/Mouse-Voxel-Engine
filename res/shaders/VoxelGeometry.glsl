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

out vec4 fColor;

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

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}
}

void makeBottomFace(const vec3 p)
{
	const vec3[4] vertices = vec3[4](
		vec3(0.0, 0.0, 0.0),
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 0.0, 1.0),
		vec3(1.0, 0.0, 1.0)
	);

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}
}

void makeFrontFace(const vec3 p)
{
	const vec3[4] vertices = vec3[4](
		vec3(0.0, 0.0, 1.0),
		vec3(1.0, 0.0, 1.0),
		vec3(0.0, 1.0, 1.0),
		vec3(1.0, 1.0, 1.0)
	);

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}
}

void makeBackFace(const vec3 p)
{
	const vec3[4] vertices = vec3[4](
		vec3(0.0, 0.0, 0.0),
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0),
		vec3(1.0, 1.0, 0.0)
	);

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}
}

void makeLeftFace(const vec3 p)
{
	const vec3[4] vertices = vec3[4](
		vec3(0.0, 0.0, 0.0),
		vec3(0.0, 0.0, 1.0),
		vec3(0.0, 1.0, 0.0),
		vec3(0.0, 1.0, 1.0)
	);

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}
}

void makeRightFace(const vec3 p)
{
	const vec3[4] vertices = vec3[4](
		vec3(1.0, 0.0, 0.0),
		vec3(1.0, 0.0, 1.0),
		vec3(1.0, 1.0, 0.0),
		vec3(1.0, 1.0, 1.0)
	);

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}
}

void makeFace()
{
	vec3 pos = vec3(gl_in[0].gl_Position);
	int faces = inData[0].faces;

	fColor = inData[0].color;

	//makeFrontFace(pos + vec3(0.0, 0.0, faces));

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

	EndPrimitive();
}

void main()
{
	makeFace();
}