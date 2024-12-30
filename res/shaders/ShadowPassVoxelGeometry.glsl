#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 18) out;
//	you'll only see 3 faces of a cube at most

in VertexData {
	highp int surrounding;
} inData[];	// array of all vertices in primitive (only one cuz point)

// for camera transform
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

uniform vec3 viewPosition;

bool backFaceCull = true;

/// must be in continuity with Voxel.h

// -x : left
// +x : right
// -y : bottom
// +y : top
// -z : front
// +z : back
// ordering convention : (x, y, z)
// eg: (1, 1, 1) is right top back

const highp int LeftBottomFront			=  0;		// 0  : (-1, -1, -1)
const highp int CenterBottomFront		=  1;		// 1  : ( 0, -1, -1)
const highp int RightBottomFront		=  2;		// 2  : ( 1, -1, -1)
const highp int LeftCenterFront			=  3;		// 3  : (-1,  0, -1)
const highp int CenterCenterFront		=  4;		// 4  : ( 0,  0, -1)
const highp int RightCenterFront		=  5;		// 5  : ( 1,  0, -1)
const highp int LeftTopFront			=  6;		// 6  : (-1,  1, -1)
const highp int CenterTopFront			=  7;		// 7  : ( 0,  1, -1)
const highp int RightTopFront			=  8;		// 8  : ( 1,  1, -1)
const highp int LeftBottomCenter		=  9;		// 9  : (-1, -1,  0)
const highp int CenterBottomCenter		=  10 ;		// 10 : ( 0, -1,  0)
const highp int RightBottomCenter		=  11 ;		// 11 : ( 1, -1,  0)
const highp int LeftCenterCenter		=  12 ;		// 12 : (-1,  0,  0)
const highp int CenterCenterCenter		=  13 ;		// 13 : ( 0,  0,  0)
const highp int RightCenterCenter		=  14 ;		// 14 : ( 1,  0,  0)
const highp int LeftTopCenter			=  15 ;		// 15 : (-1,  1,  0)
const highp int CenterTopCenter			=  16 ;		// 16 : ( 0,  1,  0)
const highp int RightTopCenter			=  17 ;		// 17 : ( 1,  1,  0)
const highp int LeftBottomBack			=  18 ;		// 18 : (-1, -1,  1)
const highp int CenterBottomBack		=  19 ;		// 19 : ( 0, -1,  1)
const highp int RightBottomBack			=  20 ;		// 20 : ( 1, -1,  1)
const highp int LeftCenterBack			=  21 ;		// 21 : (-1,  0,  1)
const highp int CenterCenterBack		=  22 ;		// 22 : ( 0,  0,  1)
const highp int RightCenterBack			=  23 ;		// 23 : ( 1,  0,  1)
const highp int LeftTopBack				=  24 ;		// 24 : (-1,  1,  1)
const highp int CenterTopBack			=  25 ;		// 25 : ( 0,  1,  1)
const highp int RightTopBack			=  26 ;		// 26 : ( 1,  1,  1)

const int PrimaryVoxel = CenterCenterCenter;

const vec3[4] TOP_FACE_VERTICES = vec3[4](
	vec3(0.0, 1.0, 0.0),
	vec3(1.0, 1.0, 0.0),
	vec3(0.0, 1.0, 1.0),
	vec3(1.0, 1.0, 1.0)
);
vec3 TOP_FACE_NORMAL = vec3(0.0, 1.0, 0.0);

const vec3[4] BOTTOM_FACE_VERTICES = vec3[4](
		vec3(0.0, 0.0, 0.0),
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 0.0, 1.0),
		vec3(1.0, 0.0, 1.0)
	);
vec3 BOTTOM_FACE_NORMAL = vec3(0.0, -1.0, 0.0);

const vec3[4] FRONT_FACE_VERTICES = vec3[4](
	vec3(0.0, 0.0, 0.0),
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(1.0, 1.0, 0.0)
);
vec3 FRONT_FACE_NORMAL = vec3(0.0, 0.0, -1.0);

const vec3[4] BACK_FACE_VERTICES = vec3[4](
	vec3(0.0, 0.0, 1.0),
	vec3(1.0, 0.0, 1.0),
	vec3(0.0, 1.0, 1.0),
	vec3(1.0, 1.0, 1.0)
);
vec3 BACK_FACE_NORMAL = vec3(0.0, 0.0, 1.0);

const vec3[4] LEFT_FACE_VERTICES = vec3[4](
	vec3(0.0, 0.0, 0.0),
	vec3(0.0, 0.0, 1.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 1.0, 1.0)
);
vec3 LEFT_FACE_NORMAL = vec3(-1.0, 0.0, 0.0);

const vec3[4] RIGHT_FACE_VERTICES = vec3[4](
	vec3(1.0, 0.0, 0.0),
	vec3(1.0, 0.0, 1.0),
	vec3(1.0, 1.0, 0.0),
	vec3(1.0, 1.0, 1.0)
);
vec3 RIGHT_FACE_NORMAL = vec3(1.0, 0.0, 0.0);


/// math stuff
int toIndex(ivec3 coord, ivec3 dim)
{
    return coord.x + (coord.y * dim.x) + (coord.z * dim.y * dim.x);
}

ivec3 toCoord(int index, ivec3 dim)
{
    int x = index % dim.x;
    int y = (index / dim.x) % dim.y;
    int z = index / (dim.x * dim.y);
    return ivec3( x, y, z );
}

// takes the index of a surroundingBit and returns if that surrounding voxel is not present
// argument bitIndex must be between [0, 26]
bool isVoxelNull(int bitIndex)
{
	int surrounding = inData[0].surrounding;
	return !bool(surrounding & (1 << bitIndex));
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

void makeFace(const vec3 p, const vec3[4] vertices, const vec3 n)
{
	if (shouldCull(n, vertices[0]))
		return;

	for (int i = 0; i < 4; ++i) 
	{
		gl_Position = cameraTransform(p + vertices[i]);
		EmitVertex();
	}
	EndPrimitive();
}

void makeTopFace(const vec3 p)
{
	makeFace(p, TOP_FACE_VERTICES, TOP_FACE_NORMAL);
}

void makeBottomFace(const vec3 p)
{
	makeFace(p, BOTTOM_FACE_VERTICES, BOTTOM_FACE_NORMAL);
}

void makeFrontFace(const vec3 p)
{
	makeFace(p, FRONT_FACE_VERTICES, FRONT_FACE_NORMAL);
}

void makeBackFace(const vec3 p)
{
	makeFace(p, BACK_FACE_VERTICES, BACK_FACE_NORMAL);
}

void makeLeftFace(const vec3 p)
{
	makeFace(p, LEFT_FACE_VERTICES, LEFT_FACE_NORMAL);
}

void makeRightFace(const vec3 p)
{
	makeFace(p, RIGHT_FACE_VERTICES, RIGHT_FACE_NORMAL);
}

void makeVoxel()
{
	bool isTransparent = isVoxelNull(PrimaryVoxel);

	// if the primary voxel is transparent/null, we dont need to draw it
	if (isTransparent)
		return;

	vec3 pos = vec3(gl_in[0].gl_Position);
	
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