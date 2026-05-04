#pragma once

class GuiTexture
{
public:
	GuiTexture(int textureId, glm::vec2 position, glm::vec2 scale);
	~GuiTexture();

	int GetTextureID() const
	{
		return textureID;
	}

	glm::vec2 GetPosition() const
	{
		return position;
	}

	glm::vec2 GetScale() const
	{
		return scale;
	}

private:
	int textureID;
	glm::vec2 position;
	glm::vec2 scale;
};