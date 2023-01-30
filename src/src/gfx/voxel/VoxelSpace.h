#pragma once

#include <vector>

#include <glm/vec3.hpp>

#include "Voxel.h"

class VoxelSpace
{
public:
	// setters
	void setVoxel(const glm::ivec3& position, Voxel v);
	void setVoxel(int index, Voxel v);

	void setDimensions(const glm::ivec3& dimensions);
	void setTransformOrigin(const glm::ivec3& transformOrigin);

	// getters
	Voxel getVoxel(const glm::ivec3& position) const;
	Voxel getVoxel(int index) const;
	const std::vector<Voxel>& getVoxels() const;

	const glm::ivec3& getDimensions() const;
	const glm::ivec3& getTransformOrigin() const;
	int getSize() const;

private:
	std::vector<Voxel> voxels;
	glm::ivec3 dimensions;
	glm::ivec3 transformOrigin;	// where the origin for transformations should be

public:
	// copies elements of current vector into a new one after resizing
	void updateVector(const glm::ivec3& newDimensions);
};