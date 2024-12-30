#pragma once

#include "../voxel/VoxelGrid.h"

#include "gl/VAO.h"
#include "gl/VertexAttributes.h"

#include <memory>

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

public:
    // Shader Attributes
    inline static constexpr VertexAttributeMetaData POSITION_ATTRIBUTE{
        0,                  // location
        3,                  // numElements
        GL_FLOAT,           // elementDataType
        GL_FALSE,           // shouldNormalize
        sizeof(glm::vec3),  // stride
        0,                  // offset
        false               // integerBased
    };
    inline static constexpr VertexAttributeMetaData COLOR_ATTRIBUTE{
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(VoxelGraphicsData),
        offsetof(VoxelGraphicsData, VoxelGraphicsData::color),
        false
    };
    inline static constexpr VertexAttributeMetaData SURROUNDING_ATTRIBUTE{
        2,
        1,
        GL_INT,
        GL_FALSE,
        sizeof(GLint),
        0,
        true
    };

};