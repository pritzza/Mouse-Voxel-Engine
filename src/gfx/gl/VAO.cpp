#include "VAO.h"

#include <cassert>

VAO::VAO()
{
    glGenVertexArrays(1, &handle);
}

VAO::VAO(VAO&& other)
{
    *this = std::move(other);
}

VAO& VAO::operator=(VAO&& other)
{
    destroy();
    vbos = std::move(other.vbos);
    handle = other.handle;
    indexBuffer = std::move(other.indexBuffer);
    other.reset();

    return *this;
}

VAO::~VAO()
{
    destroy();
}

void VAO::defineIBO(const std::vector<GLuint>& indices)
{
    // todo: implement
    //indexBuffer = std::make_unique<IBO>(new IBO{ indices });
    assert(false);
}

void VAO::reset()
{
    vbos.clear();
    handle = 0;
    indexBuffer = nullptr;
}

void VAO::destroy()
{
    glDeleteVertexArrays(1, &handle);
}
