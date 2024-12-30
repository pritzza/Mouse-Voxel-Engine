#pragma once

#include <glad/glad.h>

#include <optional>

class Window;

class FBO
{
public:
    static void bindDefault();
    inline static const Window* applicationContextWindow{ nullptr };

public:
    FBO(int width, int height);

    // delete non move constructor and assignment operator to prevent
    // unintentional resource leaks
    FBO(const FBO&) = delete;
    FBO& operator= (const FBO&) = delete;

    // dont want to deal with these either
    FBO(FBO&&) = delete;
    FBO& operator= (FBO&&) = delete;

    ~FBO();


    void createTexture();
    void bindTexture();

    void createDepthBuffer();
    void bindDepthBuffer();

    void createStencilBuffer();
    void bindStencilBuffer();


    void bind();
    void unbind();


    std::optional<GLuint> getTexture() const        { return textureHandle.value();  }
    std::optional<GLuint> getDepthBuffer() const    { return depthHandle.value();    }
    std::optional<GLuint> getStencilBuffer() const  { return stencilHandle.value();  }

    int getWidth() const                            { return width;                  }
    int getHeight() const                           { return height;                 }

private:
    GLuint handle;
    std::optional<GLuint> textureHandle;    // all empty be default 
    std::optional<GLuint> depthHandle;      
    std::optional<GLuint> stencilHandle;   

    const int width;
    const int height;
};