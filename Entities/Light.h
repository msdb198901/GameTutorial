#pragma once

#include <glm/glm.hpp>

class Light
{
public:
	Light( glm::vec3 position, glm::vec3 color );
	~Light();

	void SetColor(glm::vec3 color);
	void SetPosition(glm::vec3 position);
	glm::vec3 GetColor();
	glm::vec3 GetPosition();

private:
	glm::vec3 m_Color;
	glm::vec3 m_Position;
};