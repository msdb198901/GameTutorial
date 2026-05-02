#pragma once

class RawModel;
class Texture;
class Loader;
class Terrain
{
public:
	Terrain(int gridX, int gridZ, Loader *loader, Texture* texture);
	~Terrain();

	RawModel* GenerateTerrain(Loader *loader);

	float GetX() { return x; }
	float GetZ() { return z; }
	RawModel*  GetRawModel() { return model; }
	Texture* GetTexture() { return texture; }

private:
	static float SIZE;
	static int VERTEX_COUNT;

	float x;
	float z;
	RawModel* model;
    Texture* texture;
};