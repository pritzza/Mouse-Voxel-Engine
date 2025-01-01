#include "VoxelModel.h"

#include "VoxelShader.h"

#include <iostream>

VoxelModel::VoxelModel(const std::shared_ptr<VoxelGrid>& voxels)
{
	create(voxels);
}

void VoxelModel::create(const std::shared_ptr<VoxelGrid>& voxels)
{
	voxelGrid = voxels;

	vao.bind();

	VoxelGrid& vg{ *voxelGrid.get() };

	// TODO clean up/relocate/abstract compression logic
	const std::vector<VoxelGraphicsData>& denseGraphicsData{ vg.getGraphicsData()};
	const std::vector<SurroundingVoxels>& denseSurroundingData{ vg.getSurroundingData() };
	const std::vector<glm::vec3>& densePositionData{ vg.getPositionData() };

	std::vector<VoxelGraphicsData>	sparseGraphicsData;
	std::vector<SurroundingVoxels>  sparseSurroundingData;
	std::vector<glm::vec3>			sparsePositionData;

	if (uploadDataSparsely)
	{
		for (int i = 0; i < vg.getSize(); ++i)
		{
			const VoxelGraphicsData gd{ denseGraphicsData[i] };
			const int surrounding{ denseSurroundingData[i] };

			// conditions to not send a voxel to gpu
			const bool empty{ vg.get(i).id == Voxel::ID::Null };
			const bool fullyObscured{ surrounding == Voxel::DenselySurrounded };

			if (!empty && !fullyObscured)
			{
				sparseGraphicsData.emplace_back(denseGraphicsData[i]);
				sparseSurroundingData.emplace_back(denseSurroundingData[i]);
				sparsePositionData.emplace_back(densePositionData[i]);
			}
		}

		std::cout << sparseGraphicsData.size() << " vs " << denseGraphicsData.size() << '\n';
	}

	const std::vector<VoxelGraphicsData>* graphicsDataSource{ nullptr };
	const std::vector<SurroundingVoxels>* surroundingDataSource{ nullptr };
	const std::vector<glm::vec3>*		  positionDataSource{ nullptr };

	if (uploadDataSparsely)
	{
		graphicsDataSource    = &sparseGraphicsData;
		surroundingDataSource = &sparseSurroundingData;
		positionDataSource    = &sparsePositionData;
	}
	else
	{
		graphicsDataSource    = &denseGraphicsData;
		surroundingDataSource = &denseSurroundingData;
		positionDataSource    = &densePositionData;
	}


	vao.defineAttribute(*positionDataSource, VoxelModel::POSITION_ATTRIBUTE);
	vao.defineAttribute(*graphicsDataSource, VoxelModel::COLOR_ATTRIBUTE);
	vao.defineAttribute(*surroundingDataSource, VoxelModel::SURROUNDING_ATTRIBUTE);

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

	// TODO clean up/relocate/abstract compression logic
	const std::vector<VoxelGraphicsData>& denseGraphicsData{ vg.getGraphicsData()};
	const std::vector<SurroundingVoxels>& denseSurroundingData{ vg.getSurroundingData() };
	const std::vector<glm::vec3>& densePositionData{ vg.getPositionData() };

	std::vector<VoxelGraphicsData>	sparseGraphicsData;
	std::vector<SurroundingVoxels>  sparseSurroundingData;
	std::vector<glm::vec3>			sparsePositionData;

	if (uploadDataSparsely)
	{
		for (int i = 0; i < vg.getSize(); ++i)
		{
			const int surrounding{ denseSurroundingData[i] };

			// if a voxel has all of its neighbors, dont even send it to GPU
			if (surrounding != Voxel::DenselySurrounded)
			{
				sparseGraphicsData.emplace_back(denseGraphicsData[i]);
				sparseSurroundingData.emplace_back(denseSurroundingData[i]);
				sparsePositionData.emplace_back(densePositionData[i]);
			}
		}

		std::cout << sparseGraphicsData.size() << " vs " << denseGraphicsData.size() << '\n';
	}

	const std::vector<VoxelGraphicsData>* graphicsDataSource{ nullptr };
	const std::vector<SurroundingVoxels>* surroundingDataSource{ nullptr };
	const std::vector<glm::vec3>*		  positionDataSource{ nullptr };

	if (uploadDataSparsely)
	{
		graphicsDataSource    = &sparseGraphicsData;
		surroundingDataSource = &sparseSurroundingData;
		positionDataSource    = &sparsePositionData;
	}
	else
	{
		graphicsDataSource    = &denseGraphicsData;
		surroundingDataSource = &denseSurroundingData;
		positionDataSource    = &densePositionData;
	}

	if (isGraphicsDataStale)
	{
		graphicsDataBuffer->updateBuffer(0, *graphicsDataSource);
		isGraphicsDataStale = false;
	}
	if (isSurroundingDataStale)
	{
		surroundingDataBuffer->updateBuffer(0, *surroundingDataSource);
		isSurroundingDataStale = false;
	}
	if (isPositionDataStale)
	{
		positionDataBuffer->updateBuffer(0, *positionDataSource);
		isSurroundingDataStale = false;
	}
}
