#pragma once

#include "VoxelGrid.h"

#include "gfx/ModelMatrix.h"
#include "gfx/gl/VAO.h"

#include <memory>

class ShaderProgram;

class VoxelModel
{
public:
	ModelMatrix modelMatrix;

public:
	VoxelModel() {}
	VoxelModel(const std::shared_ptr<VoxelGrid>& voxels);

	void create(const std::shared_ptr<VoxelGrid>& voxels);

	void render(const ShaderProgram& voxelShader) const;

	void updateBuffers() const;

private:
	VAO vao;
	VBO* graphicsDataBuffer;
	VBO* positionDataBuffer;
	VBO* surroundingDataBuffer;
	std::shared_ptr<VoxelGrid> voxelGrid;

	bool created{ false };
};