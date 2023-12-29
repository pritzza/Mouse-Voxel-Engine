#pragma once

#include "../gfx/gl/ShaderProgram.h"

class VoxelModel;

class VoxelShader
{
public:
    VoxelShader();

    void use() const;
    void unuse() const;

    void update(
        const glm::mat4& viewMat,
        const glm::mat4& perspectiveMat,
        const glm::vec3& viewPos,
        float time
    );

    void setModelMatrix(const glm::mat4& modelMat) const;

private:
    ShaderProgram program;

private:
    void setViewMatrix(const glm::mat4& viewMat);
    void setPerspectiveMatrix(const glm::mat4& perspectiveMat);
    void setViewPosition(const glm::vec3& pos);
    void setTime(float time);

private:
    using FilePath = std::string_view;
    
    // shader paths
    static constexpr FilePath VERTEX_SHADER_PATH
    {
        "res/shaders/VoxelVertex.glsl"
    };
    static constexpr FilePath GEOMETRY_SHADER_PATH
    {
        "res/shaders/VoxelGeometry.glsl"
    };
    static constexpr FilePath FRAGMENT_SHADER_PATH
    {
        "res/shaders/VoxelFragment.glsl"
    };

    /// uniforms
    using UniformName = std::string_view;

    // camera transform
    static constexpr UniformName UNIFORM_MODEL_MAT{ "model" };
    static constexpr UniformName UNIFORM_VIEW_MAT{ "view" };
    static constexpr UniformName UNIFORM_PERSPECTIVE_MAT{ "perspective" };

    static constexpr UniformName UNIFORM_VIEW_POSITION{ "viewPosition" };

    static constexpr UniformName UNIFORM_TIME{ "time" };

    inline static const std::vector<UniformName> UNIFORM_NAMES
    {
        UNIFORM_MODEL_MAT,
        UNIFORM_VIEW_MAT,
        UNIFORM_PERSPECTIVE_MAT,
        UNIFORM_VIEW_POSITION,
        //UNIFORM_TIME
    };
};