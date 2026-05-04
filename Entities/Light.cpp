#include "StdAfx.h"	
#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color)
{
	m_Color = color;
	m_Position = position;
	m_Attenuation = glm::vec3(1.0f, 0.0f, 0.0f);
}

Light::Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation)
{
	m_Color = color;
	m_Position = position;
	m_Attenuation = attenuation;
}

Light::~Light()
{

}

void Light::SetPosition(glm::vec3 position)
{
	m_Position = position;
}

void Light::SetColor(glm::vec3 color)
{
	m_Color = color;
}

glm::vec3 Light::GetColor()
{
	return m_Color;
}

glm::vec3 Light::GetPosition()
{
	return m_Position;
}

glm::vec3 Light::GetAttenuation()
{
	return m_Attenuation;
}