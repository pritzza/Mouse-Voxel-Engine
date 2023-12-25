#include "VoxelGrid.h"

VoxelGrid::VoxelGrid(const glm::ivec3 dim)
	:
	dim{ dim },
	size{ dim.x * dim.y * dim.z},
	voxels{				dim, DEFAULT_VOXEL_ID			},
	voxelGraphics{		dim, DEFAULT_VOXEL_GRAPHIC		},
	voxelPositions{		dim, DEFAULT_VOXEL_POSITION		},
	surroundingVoxels{	dim, DEFAULT_VOXEL_SURROUNDINGS	}
{
	setPositions();
}

bool VoxelGrid::set(const glm::ivec3& coord, const VoxelNew& voxel)
{
	if (Math::isInside(coord, dim))
	{
		const int index{ Math::toIndex(coord, dim) };
		return set(index, voxel);
	}

	return false;
}

bool VoxelGrid::set(int index, const VoxelNew& voxel)
{
	if (!Math::isInside(index, 0, size))
		return false;

	bool isDifferentID{ voxel.id == voxels.get(index) };

	const bool worked{ voxels.set(index, voxel.id) };

	if (worked)
	{
		voxelGraphics.set(index, voxel.graphic);

		// dont need to update surrounding if only graphical change
		//if (isDifferentID)
			updateSurrounding(index);
	}

	return worked;
}

bool VoxelGrid::setID(int index, VoxelNew::ID id)
{
	const bool worked = voxels.set(index, id);

	if (worked)
		updateSurrounding(index);

	return worked;
}

bool VoxelGrid::setGraphic(int index, const Color& color)
{
	return voxelGraphics.set(index, color);
}

VoxelNew VoxelGrid::get(int index) const
{
	VoxelNew::ID id = voxels.get(index);
	SurroundingVoxels surrounding = surroundingVoxels.get(index);
	Color color = voxelGraphics.get(index);
	return VoxelNew(id, color, surrounding.surrounding);
}

void VoxelGrid::update()
{

}

const std::vector<Color>& VoxelGrid::getGraphicsData() const
{
	return voxelGraphics.getData();
}

const std::vector<Position>& VoxelGrid::getPositionData() const
{
	return voxelPositions.getData();
}

const std::vector<SurroundingVoxels>& VoxelGrid::getSurroundingData() const
{
	return surroundingVoxels.getData();
}

void VoxelGrid::setPositions()
{
	for (int i = 0; i < size; ++i)
	{
		const glm::ivec3 coord{ Math::toCoord(i, dim) };
		voxelPositions.set(i, Position( coord.x, coord.y, coord.z ));
	}
}

void VoxelGrid::updateSurrounding(const glm::ivec3& coord)
{
	const auto surroundingOffsets = Math::getCubeSurroundingCoords();
	for (const glm::ivec3& surroundingOffset : surroundingOffsets)
	{
		if (!Math::isInside(coord + surroundingOffset, dim))
			continue;

		bool bit{ voxels.get(coord) == VoxelNew::ID::Filled };
		setSurroundingBit(bit, surroundingOffset, coord);
	}
}

void VoxelGrid::updateSurrounding(int index)
{
	updateSurrounding(Math::toCoord(index, dim));
}

void VoxelGrid::setSurroundingBit(
	bool value, 
	const glm::ivec3& offset, 
	const glm::ivec3& center
)
{
	static const glm::ivec3 DIM{ 3, 3, 3 };
	static const glm::ivec3 UNCENTER{ 1, 1, 1 };

	// the coordinate of the voxel whose data we're actually changing
	const glm::ivec3 changingCoord{ center + offset };

	SurroundingVoxels data{ surroundingVoxels.get(changingCoord) };

	// bit corresponds to the index of the offset in a 
	// 3^3 grid where index 0 is (0,0,0)
	const int bitIndex{ Math::toIndex(offset + UNCENTER, DIM) };

	if (value)
		data.surrounding |= (1 << bitIndex);	// set bit to 1
	else 
		data.surrounding &= ~(1 << bitIndex);	// set bit to 0

	surroundingVoxels.set(changingCoord, data);
}
