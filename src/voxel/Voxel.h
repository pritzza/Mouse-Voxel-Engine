#pragma once

#include <glm/vec4.hpp>
#include "../gfx/VertexAttributes.h"

struct VoxelGraphicsData
{
	glm::vec4 color;
};

struct Voxel {
	enum class ID
	{
		Null,
		Filled
	};

	ID id;
	//VoxelGraphicsData graphic;
	Color graphic;
	int surrounding;

	enum Surrounding
	{
		// -x : left
		// +x : right
		// -y : bottom
		// +y : top
		// -z : front
		// +z : back
		// ordering convention : (x, y, z)
		// eg: (1, 1, 1) is right top back
		
		LeftBottomFront		= 1 << 0,	// 0  : (-1, -1, -1)
		CenterBottomFront	= 1 << 1,	// 1  : ( 0, -1, -1)
		RightBottomFront	= 1 << 2,	// 2  : ( 1, -1, -1)
		LeftCenterFront		= 1 << 3,	// 3  : (-1,  0, -1)
		CenterCenterFront	= 1 << 4,	// 4  : ( 0,  0, -1)
		RightCenterFront	= 1 << 5,	// 5  : ( 1,  0, -1)
		LeftTopFront		= 1 << 6,	// 6  : (-1,  1, -1)
		CenterTopFront		= 1 << 7,	// 7  : ( 0,  1, -1)
		RightTopFront		= 1 << 8,	// 8  : ( 1,  1, -1)
		LeftBottomCenter	= 1 << 9,	// 9  : (-1, -1,  0)
		CenterBottomCenter	= 1 << 10,	// 10 : ( 0, -1,  0)
		RightBottomCenter	= 1 << 11,	// 11 : ( 1, -1,  0)
		LeftCenterCenter	= 1 << 12,	// 12 : (-1,  0,  0)
		CenterCenterCenter	= 1 << 13,	// 13 : ( 0,  0,  0)
		RightCenterCenter	= 1 << 14,	// 14 : ( 1,  0,  0)
		LeftTopCenter		= 1 << 15,	// 15 : (-1,  1,  0)
		CenterTopCenter		= 1 << 16,	// 16 : ( 0,  1,  0)
		RightTopCenter		= 1 << 17,	// 17 : ( 1,  1,  0)
		LeftBottomBack		= 1 << 18,	// 18 : (-1, -1,  1)
		CenterBottomBack	= 1 << 19,	// 19 : ( 0, -1,  1)
		RightBottomBack		= 1 << 20,	// 20 : ( 1, -1,  1)
		LeftCenterBack		= 1 << 21,	// 21 : (-1,  0,  1)
		CenterCenterBack	= 1 << 22,	// 22 : ( 0,  0,  1)
		RightCenterBack		= 1 << 23,	// 23 : ( 1,  0,  1)
		LeftTopBack			= 1 << 24,	// 24 : (-1,  1,  1)
		CenterTopBack		= 1 << 25,	// 25 : ( 0,  1,  1)
		RightTopBack		= 1 << 26,	// 26 : ( 1,  1,  1)
		Primary = CenterCenterCenter
	};
};
