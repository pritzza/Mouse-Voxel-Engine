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

bool VoxelGrid::setVoxel(const glm::ivec3& coord, const Voxel& voxel)
{
	if (Math::isInside(coord, dim))
	{
		const int index{ Math::toIndex(coord, dim) };
		return setVoxel(index, voxel);
	}

	return false;
}

bool VoxelGrid::setVoxel(int index, const Voxel& voxel)
{
	if (!Math::isInside(index, 0, size))
		return false;

	const bool worked{ this->setID(index, voxel.id) };

	if (worked)
		this->setGraphic(index, voxel.graphic);

	return worked;
}

bool VoxelGrid::setID(const glm::ivec3& coord, Voxel::ID id)
{
	if (Math::isInside(coord, dim))
	{
		const int index{ Math::toIndex(coord, dim) };
		return setID(index, id);
	}

	return false;
}

bool VoxelGrid::setID(int index, Voxel::ID id)
{
	const bool isDifferentID{ id == voxels.get(index) };
	const bool worked{ voxels.set(index, id) };

	if (worked)
	{
		// premature optimization to only update surroundings if needed
		//if (isDifferentID)
			updateSurrounding(index);
	}

	return worked;
}

bool VoxelGrid::setGraphic(const glm::ivec3& coord, const Color& color)
{
	if (Math::isInside(coord, dim))
	{
		const int index{ Math::toIndex(coord, dim) };
		return setGraphic(index, color);
	}

	return false;
}

bool VoxelGrid::setGraphic(int index, const Color& color)
{
	return voxelGraphics.set(index, color);
}

Voxel VoxelGrid::get(int index) const
{
	Voxel::ID id = voxels.get(index);
	SurroundingVoxels surrounding = surroundingVoxels.get(index);
	Color color = voxelGraphics.get(index);
	return Voxel(id, color, surrounding.surrounding);
}

bool VoxelGrid::wasGraphicsDataAltered() const
{
	return voxelGraphics.wasAltered();
}

bool VoxelGrid::wasPositionDataAltered() const
{
	return voxelPositions.wasAltered();
}

bool VoxelGrid::wasSurroundingDataAltered() const
{
	return surroundingVoxels.wasAltered();
}

void VoxelGrid::ammendAlterations()
{
	ammendGraphicsDataAlterations();
	ammendPositionDataAlterations();
	ammendSurroundingDataAlterations();
}

void VoxelGrid::printSurroundingDebugInfo(bool conciseReport) const
{
	auto s = getSurroundingData();

	for (int i = 0; i < s.size(); ++i)
	{
		std::cout << i << ": ";

		const int n{ s[i].surrounding };

		for (int j = sizeof(int) * 8 - 1; j >= 0; --j)
		{
			std::cout << ((n >> j) & 1);
		}
		std::cout << "\n";
	}

	if (conciseReport)
		return;

	auto getIndexName = [](int i) -> std::string {
		switch (i) {
		case 0: return "LeftBottomFront";
		case 1: return "CenterBottomFront";
		case 2: return "RightBottomFront";
		case 3: return "LeftCenterFront";
		case 4: return "CenterCenterFront";
		case 5: return "RightCenterFront";
		case 6: return "LeftTopFront";
		case 7: return "CenterTopFront";
		case 8: return "RightTopFront";
		case 9: return "LeftBottomCenter";
		case 10: return "CenterBottomCenter";
		case 11: return "RightBottomCenter";
		case 12: return "LeftCenterCenter";
		case 13: return "CenterCenterCenter";
		case 14: return "RightCenterCenter";
		case 15: return "LeftTopCenter";
		case 16: return "CenterTopCenter";
		case 17: return "RightTopCenter";
		case 18: return "LeftBottomBack";
		case 19: return "CenterBottomBack";
		case 20: return "RightBottomBack";
		case 21: return "LeftCenterBack";
		case 22: return "CenterCenterBack";
		case 23: return "RightCenterBack";
		case 24: return "LeftTopBack";
		case 25: return "CenterTopBack";
		case 26: return "RightTopBack";
		default: return "Unknown";
		}
	};

	for (int i = 0; i < s.size(); ++i)
	{
		glm::ivec3 c{ Math::toCoord(i, dim) };
		std::cout << "Voxel " << i <<
			" (" << c.x << ", " << c.y << ", " << c.z
			<< ")  has the following bits set : \n";

		const int n{ s[i].surrounding };

		std::vector<int> setIndices;
		for (int j = sizeof(int) * 8 - 1; j >= 0; --j)
			if ((n >> j) & 1)
				setIndices.push_back(j);

		std::sort(setIndices.begin(), setIndices.end());

		for (int j : setIndices)
			std::cout << "(" << j << ", " << getIndexName(j) << ")\n";

		std::cout << "\n";
	}
}

void VoxelGrid::ammendGraphicsDataAlterations()
{
	voxelGraphics.ammendAlterations();
}

void VoxelGrid::ammendPositionDataAlterations()
{
	voxelPositions.ammendAlterations();
}

void VoxelGrid::ammendSurroundingDataAlterations()
{
	surroundingVoxels.ammendAlterations();
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
	const bool bit{ voxels.get(coord) == Voxel::ID::Filled };
	const auto surroundingOffsets = Math::getCubeSurroundingCoords();
	
	for (const glm::ivec3& surroundingOffset : surroundingOffsets)
	{
		if (!Math::isInside(coord + surroundingOffset, dim))
			continue;

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
	const int bitIndex{ Math::toIndex(UNCENTER - offset, DIM) };

	if (value)
	{
		data.surrounding |= (1 << bitIndex);	// set bit to 1
		//std::cout << "set " << bitIndex << " of (" << cc.x << ", " << cc.y << ", " << cc.z << ")\n";
	}
	else
	{
		data.surrounding &= ~(1 << bitIndex);	// set bit to 0
		//std::cout << "unset " << bitIndex << " of (" << cc.x << ", " << cc.y << ", " << cc.z << ")\n";
	}

	surroundingVoxels.set(changingCoord, data);
}
