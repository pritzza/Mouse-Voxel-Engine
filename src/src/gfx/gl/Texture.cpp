#include "Texture.h"

#include <glad/glad.h>

#include <stb_image.h>

#include <iostream>

Texture::Texture(const std::string_view& path, GLuint targetTexture)
{
    init(path, targetTexture);
}

Texture::~Texture()
{
    glDeleteTextures(1, &handle);
}

void Texture::init(const std::string_view& path, GLuint targetTexture)
{
    this->targetTexture = targetTexture;

    glGenTextures(1, &handle);

    /// load texture from disk
    // prep for loading texture data
    int width{ -1 };
    int height{ -1 };
    int numColorChannels{ -1 };

    // load texture data
    unsigned char* const pixelData{ stbi_load(
        path.data(),
        &width,
        &height,
        &numColorChannels,
        0                       // desired channels (0 does it automatically)
    ) };

    // if texture data not loaded
    if (pixelData == nullptr)
        std::cerr << "Error! Couldn't load texture at " << path << '\n';
    else
    {
        bind();

        bufferData(width, height, numColorChannels, pixelData);

        // free texture data resources
        stbi_image_free(pixelData);
    }
}

void Texture::configure(
    GLuint horizontalWrap, 
    GLuint verticalWrap, 
    GLuint minFilter, 
    GLuint magFilter
)
{
    bind();

    glTexParameteri(targetTexture, GL_TEXTURE_WRAP_S, horizontalWrap);
    glTexParameteri(targetTexture, GL_TEXTURE_WRAP_T, verticalWrap);

    glTexParameteri(targetTexture, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(targetTexture, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::apply(GLuint textureUnit)
{
    // active texture unit
    glActiveTexture(textureUnit);

    // bind texture to said active texture unit
    bind();
}

void Texture::bind() const
{
    glBindTexture(targetTexture, handle);
}

void Texture::bufferData(
    int width, 
    int height, 
    int numChannels, 
    const unsigned char* const data
)
{
    GLuint internalImageFormat{ 0 };

    switch (numChannels)
    {
        case (3):   internalImageFormat = GL_RGB;   break;
        case (4):   internalImageFormat = GL_RGBA;  break;

        default:
            std::cerr << "Error! Weird image format!\n";
    }

    // buffer texture data to current texture
    glTexImage2D(
        GL_TEXTURE_2D,          // Target Texture
        0,                      // Mipmap Level (if you want to set manually, else 0)
        GL_RGBA,                // Format to Store Texture
        width,                  // width
        height,                 // height
        0,                      // border (0)
        internalImageFormat,    // Image Format of Source
        GL_UNSIGNED_BYTE,       // Datatype of Image Pixels
        data                    // Pixels
    );

    // optional: generate mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
}
