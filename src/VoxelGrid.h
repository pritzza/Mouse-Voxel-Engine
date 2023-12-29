#pragma once

#include "Grid.h"

#include <glm/vec4.hpp>

#include "voxel/Voxel.h"
#include "gfx/VertexAttributes.h"

class VoxelGrid
{
public:
	VoxelGrid(const glm::ivec3 dim);

	bool set(const glm::ivec3& coord, const Voxel& voxel);
	bool set(int index, const Voxel& voxel);
	bool setID(int index, Voxel::ID id);
	bool setGraphic(int index, const Color& color);

	Voxel get(int index) const;

	// call this after making changes
	void update();

	const std::vector<Color>& getGraphicsData() const;
	const std::vector<Position>& getPositionData() const;
	const std::vector<SurroundingVoxels>& getSurroundingData() const;

	const glm::ivec3& getDim() const { return dim; }
	int getSize() const { return size; }

private:
	Grid<Voxel::ID> voxels;
	Grid<Color> voxelGraphics;
	Grid<Position> voxelPositions;
	Grid<SurroundingVoxels> surroundingVoxels;
	const glm::ivec3 dim;
	const int size;

	bool perpetuallyUpdate{ true };

private:
	void setPositions();
	void updateSurrounding(const glm::ivec3& coord);
	void updateSurrounding(int index);

	void setSurroundingBit(
		bool value, 
		const glm::ivec3& offset, 
		const glm::ivec3& center
	);

	void uploadEverythingToGPU() const;

private:
	static constexpr Voxel::ID		DEFAULT_VOXEL_ID{ Voxel::ID::Null };
	static constexpr Color				DEFAULT_VOXEL_GRAPHIC{ 1.f, 1.f, 1.f, 1.f };
	static constexpr Position			DEFAULT_VOXEL_POSITION{ 0.f,0.f,0.f };
	static constexpr SurroundingVoxels	DEFAULT_VOXEL_SURROUNDINGS{ Voxel::Surrounding::Primary };
};