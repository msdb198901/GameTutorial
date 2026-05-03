#pragma once

// 用于特定纹理渲染
class TerrainTexture;

class TerrainTexturePack
{

public:
	TerrainTexturePack(TerrainTexture *background, TerrainTexture *r, TerrainTexture *g, TerrainTexture *b);
	~TerrainTexturePack();

	TerrainTexture *GetBackgroundTexture() const;
	TerrainTexture *GetRTexture() const;
	TerrainTexture *GetGTexture() const;
	TerrainTexture *GetBTexture() const;

private:
	TerrainTexture *m_backgroundTexture;
	TerrainTexture *m_rTexture;
	TerrainTexture *m_gTexture;
	TerrainTexture *m_bTexture;
};