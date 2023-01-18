#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

void Camera::moveRelative(Direction dir, float dt)
{
	switch (dir)
	{
		case Direction::Forward:	position += forwardDirection * dt;	break;
		case Direction::Backward:	position -= forwardDirection * dt;	break;

		case Direction::Up:			position += upDirection * dt;		break;
		case Direction::Down:		position -= upDirection * dt;		break;

		case Direction::Right:		position += rightDirection * dt;	break;
		case Direction::Left:		position -= rightDirection * dt;	break;
	}
}

void Camera::moveAbsolute(const glm::vec3& dir, float dt)
{
	position += dir * dt;
}

void Camera::update()
{
	updateDirections();
	updateViewMatrix();
	updatePerspectiveMatrix();
}

void Camera::setProjectionMatrix(float fov, float ratio, float near, float far)
{
	setFOV(fov);
	setAspectRatio(ratio);
	nearPlaneDistance = near;
	farPlaneDistance = far;
}

void Camera::updateViewMatrix()
{
	const glm::vec3 targetPosition{ position + forwardDirection };
	viewMatrix = glm::lookAt(position, targetPosition, upDirection);
}

void Camera::updatePerspectiveMatrix()
{
	perspectiveProjectionMatrix = glm::perspective(
		fieldOfView,
		aspectRatio,
		nearPlaneDistance,
		farPlaneDistance
	);
}

void Camera::updateDirections()
{
	// derive forward direction of camera from its euler angles
	forwardDirection.x = cosf(yaw) * cosf(pitch);
	forwardDirection.y = sinf(pitch);
	forwardDirection.z = sinf(yaw) * cosf(pitch);

	static constexpr glm::vec3 GLOBAL_UP{ 0.f, 1.f, 0.f };

	// unitize and calculate relative up and right direction of the camera
	forwardDirection = glm::normalize(forwardDirection);
	rightDirection = glm::normalize(glm::cross(forwardDirection, GLOBAL_UP));
	upDirection = glm::normalize(glm::cross(rightDirection, forwardDirection));
}
