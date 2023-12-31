#pragma once

#include "Grid.h"

#include "voxel/Voxel.h"
#include "gfx/VertexAttributes.h"

class VoxelGrid
{
public:
	VoxelGrid(const glm::ivec3 dim);

	// todo: 
	// optimization can be writing a function for setting many things at once

	bool setVoxel(const glm::ivec3& coord, const Voxel& voxel);
	bool setVoxel(int index, const Voxel& voxel);
	bool setID(const glm::ivec3& coord, Voxel::ID id);
	bool setID(int index, Voxel::ID id);
	bool setGraphic(const glm::ivec3& coord, const Color& color);
	bool setGraphic(int index, const Color& color);

	Voxel get(int index) const;
	
	// Should call this before potentially changing
	// voxels to check if any changes were made.
	// If this isn't called code still works but then data for 
	// voxel model will be uploaded every syncToGrid() call
	void ammendAlterations();

	bool wasGraphicsDataAltered() const;
	bool wasPositionDataAltered() const;
	bool wasSurroundingDataAltered() const;

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

private:
	void setPositions();
	void updateSurrounding(const glm::ivec3& coord);
	void updateSurrounding(int index);

	void setSurroundingBit(
		bool value, 
		const glm::ivec3& offset, 
		const glm::ivec3& center
	);

	void ammendGraphicsDataAlterations();
	void ammendPositionDataAlterations();
	void ammendSurroundingDataAlterations();

private:
	static constexpr Voxel::ID			DEFAULT_VOXEL_ID{ Voxel::ID::Null };
	static constexpr Color				DEFAULT_VOXEL_GRAPHIC{ 1.f, 1.f, 1.f, 1.f };
	static constexpr Position			DEFAULT_VOXEL_POSITION{ 0.f,0.f,0.f };
	static constexpr SurroundingVoxels	DEFAULT_VOXEL_SURROUNDINGS{ Voxel::Surrounding::Primary };
};