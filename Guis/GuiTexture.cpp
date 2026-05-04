#include "StdAfx.h"
#include "GuiTexture.h"


GuiTexture::GuiTexture(int textureId, glm::vec2 position, glm::vec2 scale)
{
	this->textureID = textureId;
	this->position = position;
	this->scale = scale;
}

GuiTexture::~GuiTexture()
{ }