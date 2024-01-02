#include "VoxelShader.h"

VoxelShader::VoxelShader()
{
	program.init(
		VERTEX_SHADER_PATH,
		GEOMETRY_SHADER_PATH,
		FRAGMENT_SHADER_PATH,
		UNIFORM_NAMES
	);
}

void VoxelShader::use() const
{
	program.use();
}

void VoxelShader::unuse() const
{
	program.unuse();
}

void VoxelShader::update(
	const glm::mat4& viewMat, 
	const glm::mat4& perspectiveMat, 
	const glm::vec3& viewPos, 
	float time
)
{
	use();

	setViewMatrix(viewMat);
	setPerspectiveMatrix(perspectiveMat);
	setViewPosition(viewPos);
	setTime(time);

	unuse();
}

void VoxelShader::setModelMatrix(const glm::mat4& modelMat) const
{
	program.setUniformMat4(
		UNIFORM_MODEL_MAT,
		modelMat
	);
}

void VoxelShader::setViewMatrix(const glm::mat4& viewMat)
{
	program.setUniformMat4(
		UNIFORM_VIEW_MAT,
		viewMat
	);
}

void VoxelShader::setPerspectiveMatrix(const glm::mat4& perspectiveMat)
{
	program.setUniformMat4(
		UNIFORM_PERSPECTIVE_MAT,
		perspectiveMat
	);
}

void VoxelShader::setViewPosition(const glm::vec3& pos)
{
	program.setUniformVec3f(
		UNIFORM_VIEW_POSITION,
		pos
	);
}

void VoxelShader::setTime(float time)
{
	program.setUniformf(
		UNIFORM_TIME, 
		time
	);
}
