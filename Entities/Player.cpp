#include "StdAfx.h"
#include "Player.h"

const float Player::RUN_SPEED = 20;
const float Player::TRUN_SPEED = 160.0f;
const float Player::GRAVITY = -50;
const float Player::JUMP_SPEED = 30;
const float Player::TERRAIN_HEIGHT = 0;

Player::Player(TextureModel* model, glm::vec3 position, glm::vec3 rotation, float scale)
	: Entity(model, position, rotation, scale)
{
	m_movementSpeed = 0;
	m_rotationSpeed = 0;
}

Player::~Player()
{ }

void Player::Move(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_movementSpeed = Player::RUN_SPEED;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_movementSpeed = -Player::RUN_SPEED;
	}
	else {
		m_movementSpeed = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_rotationSpeed = Player::TRUN_SPEED;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_rotationSpeed = -Player::TRUN_SPEED;
	}
	else
	{
		m_rotationSpeed = 0;
	}

	bool isOnGround = (GetPosition().y <= Player::TERRAIN_HEIGHT + 0.1f);
	if (isOnGround && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_jumpSpeed = JUMP_SPEED;
	}

	IncreaseRotation(0, glm::radians(m_rotationSpeed * deltaTime), 0);

	float distance = m_movementSpeed * deltaTime;
	float dx = distance * sin(GetRotation().y);
	float dz = distance * cos(GetRotation().y);
	IncreasePosition(dx, 0, dz);

	m_jumpSpeed += Player::GRAVITY * deltaTime;
	IncreasePosition(0, m_jumpSpeed * deltaTime, 0);

	if (GetPosition().y < Player::TERRAIN_HEIGHT)
	{
		m_jumpSpeed = 0;
		SetPosition(glm::vec3(GetPosition().x, Player::TERRAIN_HEIGHT, GetPosition().z));
	}
}