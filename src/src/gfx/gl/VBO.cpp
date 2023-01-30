#include "VBO.h"

VBO::VBO(VBO&& other)
{
	*this = std::move(other);
}

VBO& VBO::operator=(VBO&& other)
{
	destroy();
	handle = other.handle;
	targetBuffer = other.targetBuffer;
	other.reset();
	return *this;
}

VBO::~VBO()
{
	destroy();
}

void VBO::bind() const
{
	glBindBuffer(targetBuffer, handle);
}

void VBO::reset()
{
	handle = 0;
	targetBuffer = 0;
}

void VBO::destroy()
{
	glDeleteBuffers(1, &handle);
}