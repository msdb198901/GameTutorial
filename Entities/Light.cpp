#include "StdAfx.h"	
#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color)
{
	m_Color = color;
	m_Position = position;
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