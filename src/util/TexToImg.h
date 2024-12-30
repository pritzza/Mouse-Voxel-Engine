#pragma once

#include <string>

typedef unsigned int GLuint;

namespace TexToImg 
{
    void saveFrameBuffer(GLuint textureHandle, int width, int height, const std::string& filePath);

    void saveDepthBuffer(GLuint texture, int width, int height, const std::string& filePath);

}