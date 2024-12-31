#include "ShadowMapping.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

#include "../util/Math.h"

#include <numeric>

#include <iostream>

glm::mat4 ShadowMapping::createLightViewMatrix(const glm::vec3& lightDir, const Camera& camera)
{
    glm::vec3 lightPos = computeVirutalLightPosition(camera);
    glm::vec3 target = lightPos + lightDir;

    // Use a fixed up vector
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Handle edge cases where lightDir aligns with up
    if (glm::abs(glm::dot(up, lightDir)) > 0.99f)
    {
        up = glm::vec3(1.0f, 0.0f, 0.0f); // Switch to a different axis
    }

    return glm::lookAt(lightPos, target, up);
}

glm::mat4 ShadowMapping::createLightProjectionMatrix(const glm::mat4& lightView, const Camera& camera)
{
    const std::array<glm::vec3, 8> frustumCornersWS{
        Helpers::getFrustumCornersWorldSpace(camera.getProjectionMatrix(), camera.getViewMatrix())
    };

    const std::array<glm::vec3, 2> lsBoundingBox = Helpers::getLSBoundingBox(frustumCornersWS, lightView);

    const glm::vec3 minBounds{ lsBoundingBox[0] };
    const glm::vec3 maxBounds{ lsBoundingBox[1] };

    constexpr float DEPTH_MARGIN{ 0.1f };

    return glm::ortho(
        minBounds.x, maxBounds.x,   // left, right
        minBounds.y, maxBounds.y,   // bottom, top
        minBounds.z - DEPTH_MARGIN, maxBounds.z + DEPTH_MARGIN    // near, far
    );
}

glm::vec3 ShadowMapping::computeVirutalLightPosition(const Camera& camera)
{
    const float clippingCenterDist = (camera.getFarClippingPlane() + camera.getNearClippingPlane()) / 2.0;

    const glm::vec3 frustumCenter{
         clippingCenterDist * camera.getForwardDirection() + camera.getPosition()
    };

    return frustumCenter;
}

// authored with help from ChatGPT
std::array<glm::vec3, 8> ShadowMapping::Helpers::getFrustumCornersWorldSpace(
    const glm::mat4& projection, 
    const glm::mat4& view
    )
{
    std::array<glm::vec3, 8> corners;

    glm::mat4 invVP = glm::inverse(projection * view);

    std::array<glm::vec3, 8> ndcCorners = {
        glm::vec3(-1, -1, -1), glm::vec3(1, -1, -1), glm::vec3(1, 1, -1), glm::vec3(-1, 1, -1),
        glm::vec3(-1, -1,  1), glm::vec3(1, -1,  1), glm::vec3(1, 1,  1), glm::vec3(-1, 1,  1)
    };

    for (int i = 0; i < 8; ++i)
    {
        glm::vec4 corner = invVP * glm::vec4(ndcCorners[i], 1.0);
        corner /= corner.w;
        corners[i] = glm::vec3(corner);
    }

    return corners;
}

std::array<glm::vec3, 2> ShadowMapping::Helpers::getLSBoundingBox(
    const std::array<glm::vec3, 8>& frustumCornersWS, 
    const glm::mat4& lightView
    )
{
    glm::vec3 minBounds{  std::numeric_limits<float>::max() };
    glm::vec3 maxBounds{ -std::numeric_limits<float>::max() };

    for (const glm::vec3 cornerWS : frustumCornersWS)
    {
        glm::vec3 cornerLS = lightView * glm::vec4(cornerWS, 1.0);
        minBounds = glm::min(minBounds, cornerLS);
        maxBounds = glm::max(maxBounds, cornerLS);
    }

    return { minBounds, maxBounds };
}
