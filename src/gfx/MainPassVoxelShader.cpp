#include "MainPassVoxelShader.h"

#include "gl/FBO.h"

void MainPassVoxelShader::update(
	const glm::mat4& viewMat, 
	const glm::mat4& projectionMat,
	const glm::vec3& viewPos,
	const glm::vec3& lightDirection,
	const FBO& depthBuffer,
	const glm::mat4& lightViewMat,
	const glm::mat4& lightProjectionMat)
{
	// TODO need more scaleable and organized way to do this- type check and such

	use();

	setViewMatrix(viewMat);
	setProjectionMatrix(projectionMat);
	
	setViewPosition(viewPos);

	setLightDirection(lightDirection);
	
	setDepthBuffer(depthBuffer);
	setLightViewMatrix(lightViewMat);
	setLightProjectionMatrix(lightProjectionMat);

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

void MainPassVoxelShader::setProjectionMatrix(const glm::mat4& projectionMat)
{
	program.setUniformMat4(
		UNIFORM_CAMERA_PROJECTION_MAT,
		projectionMat
	);
}

void MainPassVoxelShader::setLightDirection(const glm::vec3& lightDirection)
{
	program.setUniformVec3f(
		UNIFORM_LIGHT_DIRECTION,
		lightDirection
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

void MainPassVoxelShader::setLightProjectionMatrix(const glm::mat4& lightProjectionMat)
{
	program.setUniformMat4(
		UNIFORM_LIGHT_PROJECTION_MAT,
		lightProjectionMat
	);
}
