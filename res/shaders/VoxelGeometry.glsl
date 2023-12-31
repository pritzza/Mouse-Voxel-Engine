#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 18) out;
//	you'll only see 3 faces of a cube at most

in VertexData {
	vec4 color;
	highp int faces;
} inData[];	// array of all vertices in primitive (only one cuz point)

// for camera transform
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

uniform vec3 viewPosition;

out vec4 albedo;
out vec3 normal;

bool backFaceCull = true;

// must be in continuity with Voxel.h

// -x : left
// +x : right
// -y : bottom
// +y : top
// -z : front
// +z : back
// ordering convention : (x, y, z)
// eg: (1, 1, 1) is right top back

const highp int LeftBottomFront			= 1 << 0;		// 0  : (-1, -1, -1)
const highp int CenterBottomFront		= 1 << 1;		// 1  : ( 0, -1, -1)
const highp int RightBottomFront		= 1 << 2;		// 2  : ( 1, -1, -1)
const highp int LeftCenterFront			= 1 << 3;		// 3  : (-1,  0, -1)
const highp int CenterCenterFront		= 1 << 4;		// 4  : ( 0,  0, -1)
const highp int RightCenterFront		= 1 << 5;		// 5  : ( 1,  0, -1)
const highp int LeftTopFront			= 1 << 6;		// 6  : (-1,  1, -1)
const highp int CenterTopFront			= 1 << 7;		// 7  : ( 0,  1, -1)
const highp int RightTopFront			= 1 << 8;		// 8  : ( 1,  1, -1)
const highp int LeftBottomCenter		= 1 << 9;		// 9  : (-1, -1,  0)
const highp int CenterBottomCenter		= 1 << 10 ;		// 10 : ( 0, -1,  0)
const highp int RightBottomCenter		= 1 << 11 ;		// 11 : ( 1, -1,  0)
const highp int LeftCenterCenter		= 1 << 12 ;		// 12 : (-1,  0,  0)
const highp int CenterCenterCenter		= 1 << 13 ;		// 13 : ( 0,  0,  0)
const highp int RightCenterCenter		= 1 << 14 ;		// 14 : ( 1,  0,  0)
const highp int LeftTopCenter			= 1 << 15 ;		// 15 : (-1,  1,  0)
const highp int CenterTopCenter			= 1 << 16 ;		// 16 : ( 0,  1,  0)
const highp int RightTopCenter			= 1 << 17 ;		// 17 : ( 1,  1,  0)
const highp int LeftBottomBack			= 1 << 18 ;		// 18 : (-1, -1,  1)
const highp int CenterBottomBack		= 1 << 19 ;		// 19 : ( 0, -1,  1)
const highp int RightBottomBack			= 1 << 20 ;		// 20 : ( 1, -1,  1)
const highp int LeftCenterBack			= 1 << 21 ;		// 21 : (-1,  0,  1)
const highp int CenterCenterBack		= 1 << 22 ;		// 22 : ( 0,  0,  1)
const highp int RightCenterBack			= 1 << 23 ;		// 23 : ( 1,  0,  1)
const highp int LeftTopBack				= 1 << 24 ;		// 24 : (-1,  1,  1)
const highp int CenterTopBack			= 1 << 25 ;		// 25 : ( 0,  1,  1)
const highp int RightTopBack			= 1 << 26 ;		// 26 : ( 1,  1,  1)

const int PrimaryVoxel = CenterCenterCenter;


bool isVoxelNull(highp int location)
{
	highp int faces = inData[0].faces;
	return !bool(faces & location);
}

bool shouldCull(const vec3 normal, const vec3 faceOffset)
{
	if (!backFaceCull)
	 return false;

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
	bool isTransparent = isVoxelNull(PrimaryVoxel);

	// if the primary voxel is transparent/null, we dont need to draw it
	if (isTransparent)
		return;

	vec3 pos = vec3(gl_in[0].gl_Position);
	highp int faces = inData[0].faces;

	albedo = inData[0].color;

	// TODO: figure out and fix why there are horribly wrong
	// looks like everything is reversed and Z is backwards
	//bool addTop		= isVoxelNull(CenterBottomCenter);
	//bool addBottom	= isVoxelNull(CenterTopCenter);
	//bool addFront	= isVoxelNull(CenterCenterFront);
	//bool addBack	= isVoxelNull(CenterCenterBack);
	//bool addLeft	= isVoxelNull(RightCenterCenter);
	//bool addRight	= isVoxelNull(LeftCenterCenter);

	bool addTop		= isVoxelNull(CenterTopCenter);
	bool addBottom	= isVoxelNull(CenterBottomCenter);
	bool addFront	= isVoxelNull(CenterCenterFront);
	bool addBack	= isVoxelNull(CenterCenterBack);
	bool addLeft	= isVoxelNull(LeftCenterCenter);
	bool addRight	= isVoxelNull(RightCenterCenter);

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