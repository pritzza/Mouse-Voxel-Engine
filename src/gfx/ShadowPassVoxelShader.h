#pragma once

#include "gl/ShaderProgram.h"

#include "VoxelShader.h"

class ShadowPassVoxelShader : public VoxelShader
{
public:
    ShadowPassVoxelShader() { init(); }

    void update(
        const glm::mat4& viewMat,
        const glm::mat4& projectionMat,
        const glm::vec3& viewPos
    );

    void setModelMatrix(const glm::mat4& modelMat) const;

private:
    void setViewMatrix(const glm::mat4& viewMat);
    void setProjectionMatrix(const glm::mat4& projectionMat);
    void setViewPosition(const glm::vec3& pos);

private:
    constexpr FilePath GET_VERTEX_SHADER_PATH() const override
    {
        return "res/shaders/ShadowPassVoxelVertex.glsl";
    }
    constexpr FilePath GET_GEOMETRY_SHADER_PATH() const override
    {
        return "res/shaders/ShadowPassVoxelGeometry.glsl";
    }
    constexpr FilePath GET_FRAGMENT_SHADER_PATH() const override
    {
        return "res/shaders/ShadowPassVoxelFragment.glsl";
    }

    // camera transform
    static constexpr UniformName UNIFORM_MODEL_MAT{ "model" };
    static constexpr UniformName UNIFORM_VIEW_MAT{ "view" };
    static constexpr UniformName UNIFORM_PROJECTION_MAT{ "projection" };

    static constexpr UniformName UNIFORM_VIEW_POSITION{ "viewPosition" };

    const std::vector<UniformName> GET_UNIFORM_NAMES() const override
    {
        return 
        {
            UNIFORM_MODEL_MAT,
            UNIFORM_VIEW_MAT,
            UNIFORM_PROJECTION_MAT,
            UNIFORM_VIEW_POSITION,
        };
    }

};
