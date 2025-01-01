#include "Background.h"

#include <iostream>

Background::Background()
{
	shader.init(
		VERTEX_SHADER_PATH,
		std::nullopt,
		FRAGMENT_SHADER_PATH,
		UNIFORM_NAMES
	);

	vao.bind();

	vao.defineAttribute(VERTEX_COORDS, POSITION_ATTRIBUTE);
	//vao.defineIBO(INDEX_BUFFER);
}

void Background::draw(
	float currentTime,
	const glm::vec3& viewDir,
	const glm::vec3& lightDir,
	const glm::mat4& cameraProj
)
{
	shader.use();

	shader.setUniformf(UNIFORM_TIME, currentTime);
	shader.setUniformVec3f(UNIFORM_VIEW_DIR, viewDir);
	shader.setUniformVec3f(UNIFORM_LIGHT_DIR, lightDir);
	shader.setUniformMat4(UNIFORM_CAMERA_PROJ, cameraProj);

	vao.bind();
	
	glDrawArrays(GL_TRIANGLES, 0, vao.getNumVertices());

	vao.unbind();
	shader.unuse();
}
