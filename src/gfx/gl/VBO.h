#pragma once

#include <glad/glad.h>

#include <vector>

class VBO
{
public:
	// creates, binds, and buffers data to a target buffers
	template<typename T>
	VBO(const std::vector<T>& data, GLuint targetBuffer);

	// delete non move constructor and assignment operator to prevent
	// unintentional resource leaks
	VBO(const VBO&) = delete;
	VBO& operator= (const VBO&) = delete;
	
	VBO(VBO&&);
	VBO& operator= (VBO&&);

	~VBO();

	void bind() const;

	GLuint getHandle() const { return handle; }

private:
	GLuint handle;
	GLuint targetBuffer;

private:
	template<typename T>
	void bufferData(const std::vector<T>& data);

	void reset();
	void destroy();
};

template<typename T>
inline VBO::VBO(const std::vector<T>& data, GLuint targetBuffer)
	:
	targetBuffer{ targetBuffer }
{
	glGenBuffers(1, &handle);
	bufferData(data);
}

template<typename T>
inline void VBO::bufferData(const std::vector<T>& vertices)
{
	bind();

	glBufferData(
		targetBuffer,                   // target buffer
		vertices.size() * sizeof(T),    // size (in bytes)
		vertices.data(),                // data
		GL_STATIC_DRAW                  // usage
	);
}