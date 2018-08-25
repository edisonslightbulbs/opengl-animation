#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SENSITIVTY = 0.1f;
const GLfloat FOV = 45.0f;

class animation_Camera
{
public:
	~animation_Camera() 
	{

	};

	glm::vec3 camera_pos;
	glm::vec3 camera_front;
	glm::vec3 camera_up;
	glm::vec3 camera_right;
	glm::vec3 world_up;

	GLfloat yaw; // Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
	GLfloat fov;
	GLfloat pitch;
	GLfloat sensitivity;

	animation_Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : 
		camera_front(glm::vec3(0.0f, 0.0f, -1.0f)), yaw(YAW), pitch(PITCH), fov(FOV), sensitivity(SENSITIVTY)
	{
		camera_pos = pos;
		world_up = up;

		updateCameraVectors();
	}


	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
	}

private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

		camera_front = glm::normalize(front);

		camera_right = glm::normalize(glm::cross(camera_front, world_up)); // cross product is a vector perpendicular to both vectors
		camera_up = glm::normalize(glm::cross(camera_right, camera_front));
	}
};