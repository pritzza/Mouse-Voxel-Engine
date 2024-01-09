#pragma once

#include "GLTypeDefs.h"

// arguments for glVertexAttribPointer
struct VertexAttributeMetaData
{
    GLuint location;
    GLuint numElements;
    GLenum elementDataType;
    GLboolean shouldNormalize;
    GLsizei stride;             // bytes between start of one entry and the next
    GLsizei offset;
    bool integerBased;
};
