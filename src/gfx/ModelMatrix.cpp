#include "ModelMatrix.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../util/Math.h"

void ModelMatrix::update()
{
	// reset transform
	transform = Math::IDENTITY_MATRIX;

	// note, transformation matrix is SUPPOSED to be calculated SRT
    // (scale, then rotate, then transform)
    // when I do SRT, rotation occurs about the origina after translation
    // however I want my models to be rotated about their local origin
    // so instead i am doing it in this order:

	// todo: add option for orbit/rotation not around origin

	// translate
	transform = glm::translate(transform, position);

	// rotate
	transform = glm::rotate(transform, rotation.x, Math::I_HAT);
	transform = glm::rotate(transform, rotation.y, Math::J_HAT);
	transform = glm::rotate(transform, rotation.z, Math::K_HAT);

	// scale
	transform = glm::scale(transform, scaleTransform);
}

void ModelMatrix::translate(const glm::vec3& translation)
{
	position += translation;
}

void ModelMatrix::scale(const glm::vec3& scaling)
{
	scaleTransform *= scaling;
}

void ModelMatrix::scale(float scalar)
{
	scaleTransform *= scalar;
}

void ModelMatrix::rotate(const glm::vec3& rotation)
{
	this->rotation += rotation;
}

void ModelMatrix::rotate(float degrees, Math::Axis axis)
{
	switch (axis)
	{
		case Math::Axis::X: rotation.x += degrees; break;
		case Math::Axis::Y: rotation.y += degrees; break;
		case Math::Axis::Z: rotation.z += degrees; break;
	}
}
