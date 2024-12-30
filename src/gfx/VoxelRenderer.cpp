#include "VoxelRenderer.h"

#include "VoxelObject.h"

#include "MainPassVoxelShader.h"
#include "ShadowPassVoxelShader.h"

#include "VoxelShader.h"	// todo, remove when you implement instance()

void VoxelRenderer::render(const VoxelObject& object, const MainPassVoxelShader& shader)
{
	assert(object.model->isInstanciated());

	shader.use();
	
	const ModelMatrix& mm{ object.transform };
	shader.setModelMatrix(mm.getMatrix());

	const VAO& vao{ object.model->getVAO() };
	vao.bind();
	glDrawArrays(GL_POINTS, 0, vao.getNumVertices());
	
	vao.unbind();
	shader.unuse();
}

void VoxelRenderer::drawDepthMap(const VoxelObject& object, const ShadowPassVoxelShader& shadowPass)
{
	assert(object.model->isInstanciated());

	shadowPass.use();

	const ModelMatrix& mm{ object.transform };
	shadowPass.setModelMatrix(mm.getMatrix());

	const VAO& vao{ object.model->getVAO() };
	vao.bind();
	glDrawArrays(GL_POINTS, 0, vao.getNumVertices());

	vao.unbind();
	shadowPass.unuse();
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
