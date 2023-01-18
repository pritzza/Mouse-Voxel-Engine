#include "VoxelSpace.h"

void VoxelSpace::setVoxel(const glm::ivec3& position, Voxel v)
{
	const int index{
		(position.x) + 
		(position.y * dimensions.x) + 
		(position.z * dimensions.x * dimensions.y)
	};

	setVoxel(index, v);
}

void VoxelSpace::setVoxel(int index, Voxel v)
{
	voxels[index] = v;
}

void VoxelSpace::setDimensions(const glm::ivec3& dimensions)
{
	updateVector(dimensions);

	this->dimensions = dimensions;
}

void VoxelSpace::setTransformOrigin(const glm::ivec3& transformOrigin)
{
	this->transformOrigin = transformOrigin;
}

Voxel VoxelSpace::getVoxel(const glm::ivec3& position) const
{
	const int index{
		(position.x) +
		(position.y * dimensions.x) +
		(position.z * dimensions.x * dimensions.y)
	};

	return getVoxel(index);
}

Voxel VoxelSpace::getVoxel(int index) const
{
	return voxels[index];
}

const std::vector<Voxel>& VoxelSpace::getVoxels() const
{
	return voxels;
}

const glm::ivec3& VoxelSpace::getDimensions() const
{
	return dimensions;
}

const glm::ivec3& VoxelSpace::getTransformOrigin() const
{
	return transformOrigin;
}

int VoxelSpace::getSize() const
{
	return voxels.size();
}

void VoxelSpace::updateVector(const glm::ivec3& newDimensions)
{
	const size_t newSize = newDimensions.x * newDimensions.y * newDimensions.z;

	static constexpr Voxel DEFAULT_VOXEL{ Voxel::ID::Empty, 1.f, 1.f, 1.f };

	std::vector<Voxel> newVoxels = { newSize, DEFAULT_VOXEL };

	// copy all of old data into new voxels vector
	for (int x = 0; x < dimensions.x; ++x)
		for (int y = 0; y < dimensions.y; ++y)
			for (int z = 0; z < dimensions.z; ++z)
			{
				const glm::ivec3 currentVoxelPos{ x, y, z };

				const Voxel v{ getVoxel(currentVoxelPos) };

				const int index{
					(x) +
					(y * dimensions.x) +
					(z * dimensions.x * dimensions.y)
				};

				newVoxels[index] = v;
			}

	voxels = newVoxels;
}
