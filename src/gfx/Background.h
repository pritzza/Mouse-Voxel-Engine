#pragma once

#include <glm/vec2.hpp>

#include <vector>

#include "gl/VAO.h"

#include "gl/ShaderProgram.h"

// fully self contained class which represents the background of a scene 
class Background
{
public:
    Background();

    void draw(
        float currentTime,
        const glm::vec3& lightDir,
        const glm::mat4& cameraProjView
    );

private:
    ShaderProgram shader;
	VAO vao;

private:
    inline static constexpr FilePath VERTEX_SHADER_PATH
    {
        "res/shaders/BackgroundVertex.glsl"
    };

    inline static constexpr FilePath FRAGMENT_SHADER_PATH
    {
        "res/shaders/BackgroundFragment.glsl"
    };

    // camera transform
    static constexpr UniformName UNIFORM_TIME{ "time" };
    static constexpr UniformName UNIFORM_LIGHT_DIR{ "lightDir" };
    static constexpr UniformName UNIFORM_CAMERA_PROJ_VIEW{ "cameraProjView" };

    const std::vector<UniformName> UNIFORM_NAMES
    {
        UNIFORM_TIME,
        UNIFORM_LIGHT_DIR,
        UNIFORM_CAMERA_PROJ_VIEW
    };

    // Shader Attributes
    inline static constexpr VertexAttributeMetaData POSITION_ATTRIBUTE{
        0,                  // location
        2,                  // numElements
        GL_FLOAT,           // elementDataType
        GL_FALSE,           // shouldNormalize
        sizeof(glm::vec2),  // stride
        0,                  // offset
        false               // integerBased
    };

    inline static const std::vector<glm::vec2> VERTEX_COORDS{
    {  1.f,  1.f }, // top right (first triangle)
    {  1.f, -1.f }, // bottom right (first triangle)
    { -1.f,  1.f }, // top left (first triangle)

    {  1.f, -1.f }, // bottom right (second triangle)
    { -1.f, -1.f }, // bottom left (second triangle)
    { -1.f,  1.f }  // top left (second triangle)
    };

    // todo once I get IBOs implemented
    inline static const std::vector<glm::vec2> VERTEX_COORDS2{
        {  1.f,  1.f }, // top right
        {  1.f, -1.f }, // bottom right
        { -1.f, -1.f }, // bottom left
        { -1.f,  1.f }  // top left
    };

    inline static const std::vector<GLuint> INDEX_BUFFER{
        0, 1, 3, // First triangle: top right, bottom right, top left
        1, 2, 3  // Second triangle: bottom right, bottom left, top left
    };
};