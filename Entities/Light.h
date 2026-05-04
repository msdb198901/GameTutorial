#pragma once

#include <glm/glm.hpp>

class Light
{
public:
	Light( glm::vec3 position, glm::vec3 color );
	Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation);
	~Light();

	void SetColor(glm::vec3 color);
	void SetPosition(glm::vec3 position);
	glm::vec3 GetColor();
	glm::vec3 GetPosition();
	glm::vec3 GetAttenuation();

private:
	glm::vec3 m_Color;
	glm::vec3 m_Position;
	// Ë¥¼õÏµÊý
	glm::vec3 m_Attenuation;
};