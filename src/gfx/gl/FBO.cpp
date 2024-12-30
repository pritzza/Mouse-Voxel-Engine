#include "FBO.h"

#include <iostream>

#include "../Window.h"

void FBO::bindDefault() 
{
    if (!FBO::applicationContextWindow)
    {
        std::cout << "ERROR, YOU FORGOT TO LINK THE APPLICATION WINDOW TO FBO::window!!\n\n";
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, FBO::applicationContextWindow->getWidth(), FBO::applicationContextWindow->getHeight());
}

FBO::FBO(int width, int height)
    : 
    width{ width },
    height{ height } 
{
    glGenFramebuffers(1, &handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

FBO::~FBO() 
{
    if (textureHandle.has_value()) {
        glDeleteTextures(1, &textureHandle.value());
    }
    if (depthHandle.has_value()) {
        glDeleteTextures(1, &depthHandle.value());
    }
    if (stencilHandle.has_value()) {
        glDeleteRenderbuffers(1, &stencilHandle.value());
    }
    glDeleteFramebuffers(1, &handle);
}

void FBO::createTexture() 
{
    if (textureHandle.has_value()) 
    {
        std::cout << "Notice, FBO " << this << " already has a texture ("
            << textureHandle.value() << ")\n";
        return;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    bind();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    textureHandle = texture;
}

void FBO::bindTexture() 
{
    if (textureHandle.has_value()) 
        glBindTexture(GL_TEXTURE_2D, textureHandle.value());
    else 
        std::cerr << "Error: FBO " << this << " has no texture to bind.\n";
}

void FBO::createDepthBuffer() 
{
    if (depthHandle.has_value()) 
    {
        std::cout << "Notice, FBO " << this << " already has a depth buffer ("
            << depthHandle.value() << ")\n";
        return;
    }

    GLuint depth;
    glGenTextures(1, &depth);
    glBindTexture(GL_TEXTURE_2D, depth);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    bind();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

    depthHandle = depth;
}

void FBO::bindDepthBuffer() 
{
    if (depthHandle.has_value())
        glBindTexture(GL_TEXTURE_2D, depthHandle.value());
    else 
        std::cerr << "Error: FBO " << this << " has no depth buffer to bind.\n";
}

void FBO::createStencilBuffer() 
{
    if (stencilHandle.has_value()) 
    {
        std::cout << "Notice, FBO " << this << " already has a stencil buffer ("
            << stencilHandle.value() << ")\n";
        return;
    }

    GLuint stencil;
    glGenTextures(1, &stencil);
    glBindTexture(GL_TEXTURE_2D, stencil);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX8, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    bind();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencil, 0);

    stencilHandle = stencil;
}

void FBO::bindStencilBuffer() 
{
    if (stencilHandle.has_value()) 
        glBindRenderbuffer(GL_RENDERBUFFER, stencilHandle.value());
    else
        std::cerr << "Error: FBO " << this << " has no stencil buffer to bind.\n";
}

void FBO::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
    glViewport(0, 0, width, height);
}

void FBO::unbind()
{
    FBO::bindDefault();
}
