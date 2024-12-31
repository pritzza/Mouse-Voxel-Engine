#include "ShadowPassVoxelShader.h"

void ShadowPassVoxelShader::update(
	const glm::mat4& viewMat, 
	const glm::mat4& perspectiveMat, 
	const glm::vec3& viewPos
)
{
	use();

	setViewMatrix(viewMat);
	setPerspectiveMatrix(perspectiveMat);
	setViewPosition(viewPos);

	unuse();
}

void ShadowPassVoxelShader::setModelMatrix(const glm::mat4& modelMat) const
{
	program.setUniformMat4(
		UNIFORM_MODEL_MAT,
		modelMat
	);
}

void ShadowPassVoxelShader::setViewMatrix(const glm::mat4& viewMat)
{
	program.setUniformMat4(
		UNIFORM_VIEW_MAT,
		viewMat
	);
}

void ShadowPassVoxelShader::setPerspectiveMatrix(const glm::mat4& perspectiveMat)
{
	program.setUniformMat4(
		UNIFORM_PERSPECTIVE_MAT,
		perspectiveMat
	);
}

void ShadowPassVoxelShader::setViewPosition(const glm::vec3& pos)
{
	program.setUniformVec3f(
		UNIFORM_VIEW_POSITION,
		pos
	);
}
