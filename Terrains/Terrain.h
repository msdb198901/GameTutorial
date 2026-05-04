#pragma once

class RawModel;
class TerrainTexturePack;
class TerrainTexture;
class Loader;
class HeightMap;
class Terrain
{
public:
	// 四种地形纹理 + 混合贴图
	Terrain(int gridX, int gridZ, Loader *loader, 
		TerrainTexturePack* texturePack, 
		TerrainTexture* blendMap, const char* heightMapPath);
	~Terrain();

	RawModel* GenerateTerrain(Loader *loader, const char* heightMapPath);
	glm::vec3 CalculateNormal(int x, int z, HeightMap* heightMap);

	float GetHeightOfTerrain(float worldX, float worldZ);

	float GetX() { return x; }
	float GetZ() { return z; }
	RawModel*  GetRawModel() { return model; }
	TerrainTexturePack* GetTexturePack() { return texturePack; }
	TerrainTexture* GetBlendMap() { return blendMap; }

private:
	static float SIZE;

	float x;
	float z;
	RawModel* model;
	TerrainTexturePack* texturePack;
	TerrainTexture* blendMap;
	HeightMap* heightMap;
	int VERTEX_COUNT;

	float* heights;
};
