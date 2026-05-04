#include "StdAfx.h"
#include "Camera.h"
#include "Player.h"

Camera::Camera(Player* player) : m_player(player)
{
	m_position = glm::vec3(0.0f, 0, 0);
	m_yaw = 0.0f;
	m_pitch = 20.0f;
	m_roll = 0.0f;

	m_distanceFromPlayer = 50;
	m_angleAroundPlayer = 0.0f;
}

Camera::~Camera()
{
}

void Camera::Move(GLFWwindow* window, float deltaTime)
{
	CalculateCameraPosition();
	m_yaw = 180 - m_player->GetRotation().y - m_angleAroundPlayer;
}

void Camera::CalculateZoom(float yoffset)
{
	m_distanceFromPlayer -= yoffset * 1.0f;
	if (m_distanceFromPlayer < 0) m_distanceFromPlayer = 0;
}

void Camera::CalculateAngleAroundPlayer(float offsetAngleX)
{
	// dx 为鼠标左右移动量（正值表示向右移动）
	float angleChange = offsetAngleX * 0.3f;
	m_angleAroundPlayer -= angleChange;
}

void Camera::CalculatePitch(float yoffset)
{
	float pitchChange = yoffset * 0.1f;
	m_pitch -= pitchChange;  // 鼠标向上移动，俯仰角减小
	if (m_pitch > 89.0f) m_pitch = 89.0f;
	if (m_pitch < 0.1f) m_pitch = 0.1f;
}

float Camera::CalculateHorizontalDistance() const
{
	return cos(glm::radians(m_pitch)) * m_distanceFromPlayer;
}

float Camera::CalculateVerticalDistance() const
{
	return sin(glm::radians(m_pitch)) * m_distanceFromPlayer;
}

void Camera::CalculateCameraPosition()
{
	float theta = m_player->GetRotation().y + m_angleAroundPlayer;
	m_position.x = m_player->GetPosition().x - sin(glm::radians(theta)) * CalculateHorizontalDistance();
	m_position.z = m_player->GetPosition().z - cos(glm::radians(theta)) * CalculateHorizontalDistance();
	m_position.y = m_player->GetPosition().y + CalculateVerticalDistance();
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
