#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <glm/gtc/constants.hpp>

// camera is meant to personify then abstract view matrix related transforms
class Camera
{
public:
	enum class Direction
	{
		Forward,
		Backward,
		Up,
		Down,
		Left,
		Right
	};

public:
	void moveRelative(Direction dir, float distance);
	void moveAbsolute(const glm::vec3& dir);

	// creates new matrices with its current attributes
	void update();

	// getters
	const glm::vec3& getPosition() const			{ return position;			}
	const glm::vec3& getForwardDirection() const	{ return forwardDirection;	}
	const glm::vec3& getUpDirection() const			{ return upDirection;		}
	const glm::vec3& getRightDirection() const		{ return rightDirection;	}
	
	float getPitch() const					{ return pitch;		  }
	float getYaw() const					{ return yaw;		  }
	const glm::mat4& getViewMatrix() const	{ return viewMatrix;  }

	const glm::mat4& getProjectionMatrix() const { return perspectiveProjectionMatrix; }

	// setters
	void setProjectionMatrix(float fovRad, float ratio, float near, float far);
	void setProjectionMatrix();

	void setPosition(const glm::vec3& pos)	{ position = pos; }
	void setPitch(float p)					{ pitch = p;	  }
	void setYaw(float y)					{ yaw = y;		  }

	void setFOV(float fovRad)				{ fieldOfViewRad = fovRad;  }
	void setAspectRatio(float ratio)		{ aspectRatio = ratio;		}
	void setNearClippingPlane(float near)	{ nearPlaneDistance = near; }
	void setFarClippingPlane(float far)		{ farPlaneDistance = far;	}

	float getFOV()					const { return fieldOfViewRad;		}
	float getAspectRatio()			const { return aspectRatio;			}
	float getNearClippingPlane()	const { return nearPlaneDistance;	}
	float getFarClippingPlane()		const { return farPlaneDistance;	}

private:
	// to create view matrix, camera requires position and its own coordinate system
	glm::vec3 position{ 0, 0, 0 };

	// euler angles (in radians)
	float pitch{ 0 };
	float yaw{ 0 };
	float roll{ 0 };	// unused

	// camera's relative direction vectors, derrived from euler angles
	glm::vec3 forwardDirection;
	glm::vec3 upDirection;
	glm::vec3 rightDirection;

	glm::mat4 viewMatrix;

	// perspective projection attributes, required to generate perspective frustum
	float fieldOfViewRad{ glm::half_pi<float>() };
	float aspectRatio{ 1.f };
	float nearPlaneDistance{ 0.1f };
	float farPlaneDistance{ 1000.f };

	glm::mat4 perspectiveProjectionMatrix;

private:
	void updateDirections();
	void updateViewMatrix();
	void updatePerspectiveMatrix();
};