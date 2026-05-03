#pragma once

class RawModel;
class TerrainTexturePack;
class TerrainTexture;
class Loader;
class Terrain
{
public:
	// 四种地形纹理 + 混合贴图
	Terrain(int gridX, int gridZ, Loader *loader, TerrainTexturePack* texturePack, TerrainTexture* blendMap);
	~Terrain();

	RawModel* GenerateTerrain(Loader *loader);

	float GetX() { return x; }
	float GetZ() { return z; }
	RawModel*  GetRawModel() { return model; }
	TerrainTexturePack* GetTexturePack() { return texturePack; }
	TerrainTexture* GetBlendMap() { return blendMap; }

private:
	static float SIZE;
	static int VERTEX_COUNT;

	float x;
	float z;
	RawModel* model;
	TerrainTexturePack* texturePack;
	TerrainTexture* blendMap;
};