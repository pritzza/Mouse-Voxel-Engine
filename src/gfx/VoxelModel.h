#pragma once

#include "../voxel/VoxelGrid.h"

#include "gl/VAO.h"

#include <memory>

class VoxelShader;

class VoxelModel
{
public:
	VoxelModel() {}
	VoxelModel(const std::shared_ptr<VoxelGrid>& voxels);

	void create(const std::shared_ptr<VoxelGrid>& voxels);
	
	// calling this will update the VAO/VBOs of the model to be
	// up to date with the data of the underlying voxelGrid
	void syncToGrid();

	const VAO& getVAO() const { return vao; }
	bool isInstanciated() const { return instantiated; }

	std::shared_ptr<VoxelGrid> getVoxelGrid() const { return voxelGrid; }

private:
	VAO vao;
	VBO* graphicsDataBuffer{ nullptr };
	VBO* positionDataBuffer{ nullptr };
	VBO* surroundingDataBuffer{ nullptr };
	std::shared_ptr<VoxelGrid> voxelGrid;

	bool instantiated{ false };
	bool isGraphicsDataStale{ false };
	bool isPositionDataStale{ false };
	bool isSurroundingDataStale{ false };

private:
	void updateBuffers();
};