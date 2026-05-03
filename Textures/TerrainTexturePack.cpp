#include "StdAfx.h"
#include "TerrainTexture.h"
#include "TerrainTexturePack.h"

TerrainTexturePack::TerrainTexturePack(TerrainTexture *background, TerrainTexture* r, TerrainTexture* g, TerrainTexture* b)
{
	this->m_backgroundTexture = background;
	this->m_rTexture = r;
	this->m_gTexture = g;
	this->m_bTexture = b;
}

TerrainTexturePack::~TerrainTexturePack()
{
}

TerrainTexture* TerrainTexturePack::GetBackgroundTexture() const
{
	return m_backgroundTexture;
}

TerrainTexture* TerrainTexturePack::GetRTexture() const
{
	return m_rTexture;
}

TerrainTexture* TerrainTexturePack::GetGTexture() const
{
	return m_gTexture;
}

TerrainTexture* TerrainTexturePack::GetBTexture() const
{
	return m_bTexture;
}