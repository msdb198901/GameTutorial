#pragma once

#include "Entity.h"
class Player : public Entity
{
public:
	Player(TextureModel* model, glm::vec3 position, glm::vec3 rotation, float scale);
	~Player();

	void Move(GLFWwindow* window, float deltaTime);

private:
	static const float RUN_SPEED;
	static const float TRUN_SPEED;
	static const float GRAVITY;
	static const float JUMP_SPEED;
	static const float TERRAIN_HEIGHT;

	float m_movementSpeed;
	float m_rotationSpeed;
	float m_jumpSpeed;
};