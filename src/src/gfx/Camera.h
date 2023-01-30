#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

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
	void moveRelative(Direction dir, float dt);
	void moveAbsolute(const glm::vec3& dir, float dt);

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

	float getFOV() const					{ return fieldOfView; }
	const glm::mat4& getProjectionMatrix() const { return perspectiveProjectionMatrix; }

	// setters
	void setProjectionMatrix(float fov, float ratio, float near, float far);

	void setPosition(const glm::vec3& pos)	{ position = pos; }
	void setPitch(float p)					{ pitch = p;	  }
	void setYaw(float y)					{ yaw = y;		  }

	void setFOV(float fov)					{ fieldOfView = fov;   }
	void setAspectRatio(float ratio)		{ aspectRatio = ratio; }

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
	float fieldOfView{ 3.14f / 2 };
	float nearPlaneDistance{ 0.1f };
	float farPlaneDistance{ 100.f };
	float aspectRatio{ 1.f };

	glm::mat4 perspectiveProjectionMatrix;

private:
	void updateDirections();
	void updateViewMatrix();
	void updatePerspectiveMatrix();
};