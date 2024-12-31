#pragma once

#include "gl/ShaderProgram.h"

#include "VoxelShader.h"

class FBO;

class MainPassVoxelShader : public VoxelShader
{
public:
    MainPassVoxelShader() { init(); }

    // update all uniforms constant across frame
    void update(
        const glm::mat4& cameraViewMat,
        const glm::mat4& cameraProjectionMat,
        const glm::vec3& viewPos,
        const glm::vec3& lightDirection,
        const FBO& depthBuffer,
        const glm::mat4& lightViewMat,
        const glm::mat4& lightProjectionMat
    );

    void setModelMatrix(const glm::mat4& modelMat) const;

private:
    // camera transform
    void setViewMatrix(const glm::mat4& viewMat);
    void setProjectionMatrix(const glm::mat4& projectionMat);

    void setLightDirection(const glm::vec3& lightDirection);

    // for back face culling
    void setViewPosition(const glm::vec3& pos);

    // shadow mapping
    void setDepthBuffer(const FBO& depthBuffer);
    void setLightViewMatrix(const glm::mat4& lightViewMat);
    void setLightProjectionMatrix(const glm::mat4& lightProjectionMat);

private:
    FilePath GET_VERTEX_SHADER_PATH() const override
    {
        return "res/shaders/VoxelVertex.glsl";
    }
    FilePath GET_GEOMETRY_SHADER_PATH() const override
    {
        return "res/shaders/VoxelGeometry.glsl";
    }
     FilePath GET_FRAGMENT_SHADER_PATH() const override
    {
        return "res/shaders/VoxelFragment.glsl";
    }

    // camera transform
    static constexpr UniformName UNIFORM_MODEL_MAT{ "model" };
    static constexpr UniformName UNIFORM_CAMERA_VIEW_MAT{ "cameraView" };
    static constexpr UniformName UNIFORM_CAMERA_PROJECTION_MAT{ "cameraProjection" };

    static constexpr UniformName UNIFORM_VIEW_POSITION{ "viewPosition" };

    static constexpr int DEPTH_BUFFER_TEXTURE_UNIT{ 0 };
    static constexpr UniformName UNIFORM_DEPTH_BUFFER{ "depthBuffer" };

    static constexpr UniformName UNIFORM_LIGHT_VIEW_MAT{ "lightView" };
    static constexpr UniformName UNIFORM_LIGHT_PROJECTION_MAT{ "lightProjection" };

    static constexpr UniformName UNIFORM_LIGHT_DIRECTION{ "lightDirection" };

    const std::vector<UniformName> GET_UNIFORM_NAMES() const override
    {
        return
        {
            UNIFORM_MODEL_MAT,
            UNIFORM_CAMERA_VIEW_MAT,
            UNIFORM_CAMERA_PROJECTION_MAT,
            UNIFORM_VIEW_POSITION,
            UNIFORM_LIGHT_DIRECTION,
            UNIFORM_DEPTH_BUFFER,
            UNIFORM_LIGHT_VIEW_MAT,
            UNIFORM_LIGHT_PROJECTION_MAT,
        };
    }


};
