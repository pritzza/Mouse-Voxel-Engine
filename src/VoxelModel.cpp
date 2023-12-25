#include "VoxelModel.h"

#include "gfx/gl/ShaderProgram.h"

VoxelModel::VoxelModel(const std::shared_ptr<VoxelGrid>& voxels)
{
	create(voxels);
}

void VoxelModel::create(const std::shared_ptr<VoxelGrid>& voxels)
{
	voxelGrid = voxels;

	vao.bind();

	VoxelGrid vg{ *voxelGrid.get() };

	vao.defineAttribute(vg.getPositionData());
	vao.defineAttribute(vg.getGraphicsData());
	vao.defineAttribute(vg.getSurroundingData());

	positionDataBuffer = &vao.getVBOs().at(0);
	graphicsDataBuffer = &vao.getVBOs().at(1);
	surroundingDataBuffer = &vao.getVBOs().at(2);

	created = true;
}

void VoxelModel::render(const ShaderProgram& voxelShader) const
{
	assert(created);

	voxelShader.use();

	voxelShader.setUniformMat4("model", modelMatrix.getMatrix());

	vao.bind();
	glDrawArrays(GL_POINTS, 0, vao.getNumVertices());
}

void VoxelModel::updateBuffers() const
{
	assert(created);

	vao.bind();

	VoxelGrid vg{ *voxelGrid.get() };

	graphicsDataBuffer->updateBuffer(0, vg.getGraphicsData());
	surroundingDataBuffer->updateBuffer(0, vg.getSurroundingData());
}
