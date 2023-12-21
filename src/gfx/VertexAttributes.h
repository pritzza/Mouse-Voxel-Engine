#pragma once

#include "gl/GLTypeDefs.h"

#include <cstddef>

// arguments for glVertexAttribPointer
struct VertexAttributeInfo
{
    GLuint location;
    GLuint numElements;
    GLuint elementDataType;
    GLboolean shouldNormalize;
    bool integerBased;
};

struct Position
{
    static constexpr VertexAttributeInfo META
    {
        0, 3, GL_FLOAT, GL_FALSE, false
    };

    GLfloat x, y, z;
};

struct Color
{
    static constexpr VertexAttributeInfo META
    {
        1, 4, GL_FLOAT, GL_FALSE, false
    };

    GLfloat r, g, b, a;
};

struct TextureCoordinate2D
{
    static constexpr VertexAttributeInfo META
    {
        2, 2, GL_FLOAT, GL_FALSE, false
    };

    GLfloat s, t;
};

struct Normal
{
    static constexpr VertexAttributeInfo META
    {
        3, 3, GL_FLOAT, GL_FALSE, false
    };

    GLfloat x, y, z;
};

struct VoxelFaces
{
    static constexpr VertexAttributeInfo META
    {
        4, 1, GL_INT, GL_FALSE, true
    };

    GLint faces;
};

// compile time sanity check to make sure no attirbutes have the same location
static_assert(
    Position::META.location != Color::META.location &&
    Color::META.location != TextureCoordinate2D::META.location &&
    TextureCoordinate2D::META.location != Normal::META.location &&
    Normal::META.location != VoxelFaces::META.location
);

struct Vertex
{
    Position pos;
    Color color;
    TextureCoordinate2D texCoord;
};

// todo: find a way to make this more flexible/dynamic
constexpr size_t VERTEX_POS_OFFSET{ offsetof(Vertex, Vertex::pos) };
constexpr size_t VERTEX_COLOR_OFFSET{ offsetof(Vertex, Vertex::color) };
constexpr size_t VERTEX_TEXCOORD_OFFSET{ offsetof(Vertex, Vertex::texCoord) };
