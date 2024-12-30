#pragma once

#include "gl/ShaderProgram.h"

#include "VoxelShader.h"

class MainPassVoxelShader : public VoxelShader
{
public:
    MainPassVoxelShader() { init(); }

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

    // shadow mapping
    void setLightViewMatrix(const glm::mat4& lightViewMat);
    void setLightPerspectiveMatrix(const glm::mat4& lightPerspectiveMat);

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
