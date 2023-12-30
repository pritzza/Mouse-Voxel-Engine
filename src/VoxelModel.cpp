#include "VoxelModel.h"

#include "voxel/VoxelShader.h"

VoxelModel::VoxelModel(const std::shared_ptr<VoxelGrid>& voxels)
{
	create(voxels);
}

void VoxelModel::create(const std::shared_ptr<VoxelGrid>& voxels)
{
	voxelGrid = voxels;

	vao.bind();

	VoxelGrid& vg{ *voxelGrid.get() };

	vao.defineAttribute(vg.getPositionData());
	vao.defineAttribute(vg.getGraphicsData());
	vao.defineAttribute(vg.getSurroundingData());

	positionDataBuffer = &vao.getVBOs().at(0);
	graphicsDataBuffer = &vao.getVBOs().at(1);
	surroundingDataBuffer = &vao.getVBOs().at(2);

	instantiated = true;
}

void VoxelModel::syncToGrid()
{
	VoxelGrid& vg{ *voxelGrid.get() };

	isPositionDataStale = vg.wasPositionDataAltered();
	isGraphicsDataStale = vg.wasGraphicsDataAltered();
	isSurroundingDataStale = vg.wasSurroundingDataAltered();
	
	updateBuffers();
}

void VoxelModel::updateBuffers()
{
	assert(instantiated);

	vao.bind();

	VoxelGrid& vg{ *voxelGrid.get() };

	if (isGraphicsDataStale)
	{
		graphicsDataBuffer->updateBuffer(0, vg.getGraphicsData());
		isGraphicsDataStale = false;
	}
	if (isSurroundingDataStale)
	{
		surroundingDataBuffer->updateBuffer(0, vg.getSurroundingData());
		isSurroundingDataStale = false;
	}
	if (isPositionDataStale)
	{
		positionDataBuffer->updateBuffer(0, vg.getPositionData());
		isSurroundingDataStale = false;
	}
}
