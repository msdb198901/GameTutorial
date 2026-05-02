#include "StdAfx.h"
#include "Camera.h"


Camera::Camera()
{
	m_position = glm::vec3(0.0f, -2.0f, 0.0f);
	m_yaw = 0.0f;
	m_pitch = 0.0f;
	m_roll = 0.0f;
}

Camera::~Camera()
{
}

void Camera::Move(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_position.z -= 20  * deltaTime;
	} 
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_position.z += 20 * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_position.x -= 20 * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_position.x += 20 * deltaTime;
	}
}

glm::vec3 Camera::GetPosition() const 
{
	return m_position;
}

float Camera::GetYaw() const 
{
	return m_yaw;
}

float Camera::GetPitch() const
{
	return m_pitch;
}

float Camera::GetRoll() const 
{
	return m_roll;
}
