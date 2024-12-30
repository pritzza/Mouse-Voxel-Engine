#include "VoxelModel.h"

#include "VoxelShader.h"

VoxelModel::VoxelModel(const std::shared_ptr<VoxelGrid>& voxels)
{
	create(voxels);
}

void VoxelModel::create(const std::shared_ptr<VoxelGrid>& voxels)
{
	voxelGrid = voxels;

	vao.bind();

	VoxelGrid& vg{ *voxelGrid.get() };

	vao.defineAttribute(vg.getPositionData(),	 VoxelModel::POSITION_ATTRIBUTE);
	vao.defineAttribute(vg.getGraphicsData(),	 VoxelModel::COLOR_ATTRIBUTE);
	vao.defineAttribute(vg.getSurroundingData(), VoxelModel::SURROUNDING_ATTRIBUTE);

	positionDataBuffer	  = &vao.getVBOs().at(VoxelModel::POSITION_ATTRIBUTE.location);
	graphicsDataBuffer	  = &vao.getVBOs().at(VoxelModel::COLOR_ATTRIBUTE.location);
	surroundingDataBuffer = &vao.getVBOs().at(VoxelModel::SURROUNDING_ATTRIBUTE.location);

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
