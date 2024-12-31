#include "MainPassVoxelShader.h"

#include "gl/FBO.h"

void MainPassVoxelShader::update(
	const glm::mat4& viewMat, 
	const glm::mat4& perspectiveMat, 
	const glm::vec3& viewPos, 
	const FBO& depthBuffer,
	const glm::mat4& lightViewMat,
	const glm::mat4& lightPerspectiveMat)
{
	use();

	setViewMatrix(viewMat);
	setPerspectiveMatrix(perspectiveMat);
	
	setViewPosition(viewPos);
	
	setDepthBuffer(depthBuffer);
	setLightViewMatrix(lightViewMat);
	setLightPerspectiveMatrix(lightPerspectiveMat);

	unuse();
}

void MainPassVoxelShader::setModelMatrix(const glm::mat4& modelMat) const
{
	program.setUniformMat4(
		UNIFORM_MODEL_MAT,
		modelMat
	);
}

void MainPassVoxelShader::setViewMatrix(const glm::mat4& viewMat)
{
	program.setUniformMat4(
		UNIFORM_CAMERA_VIEW_MAT,
		viewMat
	);
}

void MainPassVoxelShader::setPerspectiveMatrix(const glm::mat4& perspectiveMat)
{
	program.setUniformMat4(
		UNIFORM_CAMERA_PERSPECTIVE_MAT,
		perspectiveMat
	);
}

void MainPassVoxelShader::setViewPosition(const glm::vec3& pos)
{
	program.setUniformVec3f(
		UNIFORM_VIEW_POSITION,
		pos
	);
}

void MainPassVoxelShader::setDepthBuffer(const FBO& depthBuffer)
{
	// TODO, this is kinda jank, should have some interface between depthbuffer and textures
	glActiveTexture(GL_TEXTURE0 + DEPTH_BUFFER_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, depthBuffer.getDepthBuffer().value());
}

void MainPassVoxelShader::setLightViewMatrix(const glm::mat4& lightViewMat)
{
	program.setUniformMat4(
		UNIFORM_LIGHT_VIEW_MAT,
		lightViewMat
	);
}

void MainPassVoxelShader::setLightPerspectiveMatrix(const glm::mat4& lightPerspectiveMat)
{
	program.setUniformMat4(
		UNIFORM_LIGHT_PERSPECTIVE_MAT,
		lightPerspectiveMat
	);
}
