#include "TexToImg.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>    // requires _CRT_SECURE_NO_WARNINGS

#include <glad/glad.h>

#include <iostream>
#include <vector>

// todo, function saved textures upsidedown
void TexToImg::save(GLuint texture, int width, int height, const std::string& filePath) {
    
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Allocate a buffer to hold the texture data
    std::vector<unsigned char> pixels(width * height * 3); // RGB format

    // Read the pixels from the texture
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Save the pixels to a file
    if (stbi_write_png(filePath.c_str(), width, height, 3, pixels.data(), width * 3)) {
        std::cout << "Texture saved to " << filePath << std::endl;
    }
    else {
        std::cerr << "Failed to save texture to " << filePath << std::endl;
    }
}
