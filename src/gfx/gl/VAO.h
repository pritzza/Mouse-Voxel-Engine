#pragma once

#include "VBO.h"

#include <memory>
#include "VertexAttributes.h"

class VAO
{
public:
    VAO();

    // delete non move constructor and assignment operator to prevent
    // unintentional resource leaks
    VAO(const VAO&) = delete;
    VAO& operator= (const VAO&) = delete;
    
    VAO(VAO&&);
    VAO& operator= (VAO&&);

    ~VAO();

    // buffers and attributes vector vertex attributes
    // Attribute must have a VertexAttributeInfo member defined
    // and named "META"
    template<typename Attribute>
    void defineAttribute(
        const std::vector<Attribute>& attributeData,
        const VertexAttributeMetaData& meta
    );

    // buffers and binds GL_ELEMENT_ARRAY_BUFFER to self
    void defineIBO(const std::vector<GLuint>& indices);

    void bind() const { glBindVertexArray(handle); }
    void unbind() const { glBindVertexArray(0); }

    // getters
    GLuint getHandle() const        { return handle;            }
    GLuint getNumIndices() const    { return indexBuffer->size; }
    std::vector<VBO>& getVBOs()     { return vbos; }
    size_t getNumVertices() const { return numVertices;  }

private:
	GLuint handle;

    std::vector<VBO> vbos;

    struct IBO
    {
        IBO(const std::vector<GLuint>& indices)
            :
            buffer{ indices, GL_ELEMENT_ARRAY_BUFFER },
            size{ indices.size() }
        {}

        const VBO buffer;
        const size_t size{ 0 };
    };

    std::unique_ptr<IBO> indexBuffer;
    size_t numVertices{ 0 };

private:
    void reset();
    void destroy();
};

template<typename Attribute>
void VAO::defineAttribute(
    const std::vector<Attribute>& attributeData,
    const VertexAttributeMetaData& meta
)
{
    bind();

    vbos.emplace_back(VBO{ attributeData, GL_ARRAY_BUFFER });
    numVertices = attributeData.size();

    if (meta.integerBased)
    {
        glVertexAttribIPointer(
            meta.location,            // attribute location
            meta.numElements,         // elements per attribute
            meta.elementDataType,     // element data type
            meta.stride,                   // stride (in bytes)
            (void*)meta.offset                                    // offset (in bytes)
        );
    }
    else
    {
        glVertexAttribPointer(
            meta.location,            // attribute location
            meta.numElements,         // elements per attribute
            meta.elementDataType,     // element data type
            meta.shouldNormalize,     // normalized
            meta.stride,                   // stride (in bytes)
            (void*)meta.offset                                    // offset (in bytes)
        );
    }

    // enable attribute (no reason not to if we defined an attribute)
    glEnableVertexAttribArray(meta.location);
}


