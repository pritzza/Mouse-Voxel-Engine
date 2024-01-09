#pragma once

#include "gl/ShaderProgram.h"
#include "gl/VertexAttributes.h"

#include "../voxel/Voxel.h"

class VoxelShader
{
public:
    // Shader Attributes
    inline static constexpr VertexAttributeMetaData POSITION_ATTRIBUTE{
        0,                  // location
        3,                  // numElements
        GL_FLOAT,           // elementDataType
        GL_FALSE,           // shouldNormalize
        sizeof(glm::vec3),  // stride
        0,                  // offset
        false               // integerBased
    };
    inline static constexpr VertexAttributeMetaData COLOR_ATTRIBUTE{
        1, 
        4, 
        GL_FLOAT, 
        GL_FALSE, 
        sizeof(VoxelGraphicsData), 
        offsetof(VoxelGraphicsData, VoxelGraphicsData::color), 
        false
    };    
    inline static constexpr VertexAttributeMetaData SURROUNDING_ATTRIBUTE{
        2, 
        1, 
        GL_INT, 
        GL_FALSE, 
        sizeof(GLint), 
        0, 
        true
    };

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
        UNIFORM_TIME
    };

};
