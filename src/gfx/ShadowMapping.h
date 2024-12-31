#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <array>

class Camera;

namespace ShadowMapping
{
	glm::mat4 createLightViewMatrix(const glm::vec3& lightDir, const Camera& camera);

	glm::mat4 createLightProjectionMatrix(const glm::mat4& lightView, const Camera& camera);

	namespace Helpers 
	{
		glm::vec3 computeVirutalLightPosition(const Camera& camera);

		std::array<glm::vec3, 8> getFrustumCornersWorldSpace(
			const glm::mat4& projection, 
			const glm::mat4& view
		);

		std::array<glm::vec3, 2> getLSBoundingBox(
			const std::array<glm::vec3, 8>& frustumCornersWS,
			const glm::mat4& lightView
		);
	}
};