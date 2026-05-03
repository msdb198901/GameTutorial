#pragma once

class TerrainTexture
{

public:
	TerrainTexture(int texutreID);
	~TerrainTexture();


	int GetTextureID() const
	{
		return m_textureID;
	}

private:
	int m_textureID;
};