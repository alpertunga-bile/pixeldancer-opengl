#pragma once

#include "helper.h"

namespace pixeldancer
{
	class Camera
	{
	public:
		Camera(int width = 800, int height = 600) : width(width), height(height)
		{
			firstMouse = true;
			yaw = -90.0f;
			pitch = 0.0f;
			lastX = width / 2;
			lastY = height / 2;
			fov = 45.0f;
			cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
			cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			deltaTime = 0.0f;
			lastFrame = 0.0f;
		}

		void mouseInput(GLFWwindow* window, double xpos, double ypos)
		{
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;
			lastX = xpos; lastY = ypos;

			const float sensivity = 0.1f;
			xoffset *= sensivity;
			yoffset *= sensivity;

			yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			cameraFront = glm::normalize(direction);
		}

		void scrollInput(GLFWwindow* window, double xoffset, double yoffset)
		{
			fov -= (float)yoffset;

			if (fov < 1.0f)
				fov = 1.0f;
			if (fov > 45.0f)
				fov = 45.0f;
		}

		void activateTimer()
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
		}

		glm::mat4 getViewMatrix()
		{
			return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}

	public:
		bool firstMouse;
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		float yaw;
		float pitch;
		float lastX, lastY;
		float fov;
		int width, height;
		float deltaTime, lastFrame;
	};
}