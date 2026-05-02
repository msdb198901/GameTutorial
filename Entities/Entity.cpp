#include "StdAfx.h"
#include "Entity.h"

Entity::Entity(TextureModel* model, glm::vec3 position, glm::vec3 rotation, float scale)
{
	this->model = model;
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

TextureModel* Entity::GetTextureModel() const
{
	return model;
}

glm::vec3 Entity::GetPosition() const
{
	return position;
}
glm::vec3 Entity::GetRotation() const
{
	return rotation;
}
float Entity::GetScale() const
{
	return scale;
}

void Entity::SetPosition(glm::vec3 position)
{
	this->position = position;
}

void Entity::SetRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}

void Entity::SetScale(float scale)
{
	this->scale = scale;
}

void Entity::IncreasePosition(float x, float y, float z)
{
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
	//std::cout << "IncreasePosition: " << this->position.x << ", " << this->position.y << ", " << this->position.z << std::endl;
}

void Entity::IncreaseRotation(float x, float y, float z)
{
	this->rotation.x += x;
	this->rotation.y += y;
	this->rotation.z += z;
}