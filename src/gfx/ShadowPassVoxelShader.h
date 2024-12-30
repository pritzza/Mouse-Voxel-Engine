#pragma once

#include "gl/ShaderProgram.h"

#include "VoxelShader.h"

class ShadowPassVoxelShader : public VoxelShader
{
public:
    ShadowPassVoxelShader() { init(); }

    void update(
        const glm::mat4& viewMat,
        const glm::mat4& perspectiveMat,
        const glm::vec3& viewPos,
        float time
    );

    void setModelMatrix(const glm::mat4& modelMat) const;

private:
    void setViewMatrix(const glm::mat4& viewMat);
    void setPerspectiveMatrix(const glm::mat4& perspectiveMat);
    void setViewPosition(const glm::vec3& pos);
    void setTime(float time);

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
    static constexpr UniformName UNIFORM_PERSPECTIVE_MAT{ "perspective" };

    static constexpr UniformName UNIFORM_VIEW_POSITION{ "viewPosition" };

    static constexpr UniformName UNIFORM_TIME{ "time" };

    const std::vector<UniformName> GET_UNIFORM_NAMES() const override
    {
        return 
        {
            UNIFORM_MODEL_MAT,
            UNIFORM_VIEW_MAT,
            UNIFORM_PERSPECTIVE_MAT,
            UNIFORM_VIEW_POSITION,
            UNIFORM_TIME
        };
    }

};
