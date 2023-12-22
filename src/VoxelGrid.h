#pragma once

#include "Grid.h"

#include <glm/vec4.hpp>

enum class VoxelID
{
	Null,
	Filled
};

struct VoxelGraphicsData
{
	glm::vec4 color;
};

class VoxelGrid
{
public:
	VoxelGrid(const glm::ivec3 dim);

private:
	Grid<VoxelID> voxels;
	Grid<VoxelGraphicsData> voxelGraphics;
	Grid<int> voxelFaces;
	Grid<glm::ivec3> voxelPositions;

private:
	static constexpr VoxelID			DEFAULT_VOXEL_ID{ VoxelID::Null };
	static constexpr VoxelGraphicsData  DEFAULT_VOXEL_GRAPHIC{ {glm::vec4{ 1.0 }} };
	static constexpr int				DEFAULT_VOXEL_FACES{ 0b0 };
	static constexpr glm::ivec3			DEFAULT_VOXEL_POSITION{ glm::ivec3{ 0 } };
};