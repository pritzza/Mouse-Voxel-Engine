#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

void Camera::moveRelative(Direction dir, float distance)
{
	switch (dir)
	{
		case Direction::Forward:	position += forwardDirection * distance;	break;
		case Direction::Backward:	position -= forwardDirection * distance;	break;

		case Direction::Up:			position += upDirection * distance;			break;
		case Direction::Down:		position -= upDirection * distance;			break;

		case Direction::Right:		position += rightDirection * distance;		break;
		case Direction::Left:		position -= rightDirection * distance;		break;
	}
}

void Camera::moveAbsolute(const glm::vec3& dir)
{
	position += dir;
}

void Camera::update()
{
	updateDirections();
	updateViewMatrix();
	updateProjectionMatrix();
}

void Camera::setProjectionMatrix(float fov, float ratio, float near, float far)
{
	setFOV(fov);
	setAspectRatio(ratio);
	setNearClippingPlane(near);
	setFarClippingPlane(far);

	updateProjectionMatrix();
}

void Camera::setProjectionMatrix()
{
	updateProjectionMatrix();
}

void Camera::updateViewMatrix()
{
	const glm::vec3 targetPosition{ position + forwardDirection };
	viewMatrix = glm::lookAt(position, targetPosition, upDirection);
}

void Camera::updateProjectionMatrix()
{
	projectionMatrix = glm::perspective(
		fieldOfViewRad,
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
