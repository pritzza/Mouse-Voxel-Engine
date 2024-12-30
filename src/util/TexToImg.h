#pragma once

#include <string>

typedef unsigned int GLuint;

namespace TexToImg 
{
    void save(GLuint textureHandle, int width, int height, const std::string& filePath);
}