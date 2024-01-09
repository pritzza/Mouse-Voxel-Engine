#include "VoxelRenderer.h"

#include "VoxelShader.h"
#include "VoxelObject.h"

void VoxelRenderer::render(const VoxelObject& object, const VoxelShader& shader)
{
	assert(object.model->isInstanciated());

	const ModelMatrix& mm{ object.transform };
	shader.use();
	shader.setModelMatrix(mm.getMatrix());

	const VAO& vao{ object.model->getVAO() };
	vao.bind();
	glDrawArrays(GL_POINTS, 0, vao.getNumVertices());
	
	vao.unbind();
	shader.unuse();
}

void VoxelRenderer::instance(const VoxelObject& object, const VoxelShader& shader)
{
	// todo
}

void VoxelRenderer::renderInstances()
{
	// todo
}

void VoxelRenderer::clearInstances()
{
	// todo
}
