#include "StdAfx.h"
#include "Terrain.h"
#include "Loader.h"
#include "Texture.h"

float Terrain::SIZE = 800;
int Terrain::VERTEX_COUNT = 128;

Terrain::Terrain(int gridX, int gridZ, Loader* loader, TerrainTexturePack* texturePack, TerrainTexture* blendMap)
{
	this->x = gridX * SIZE;
	this->z = gridZ * SIZE;
	this->texturePack = texturePack;
	this->blendMap = blendMap;
	this->model = GenerateTerrain(loader);
}

Terrain::~Terrain()
{

}


RawModel* Terrain::GenerateTerrain(Loader* loader)
{
    const int count = VERTEX_COUNT * VERTEX_COUNT;
    const int verticesSize = count * 3;
    const int normalsSize = count * 3;
    const int texCoordsSize = count * 2;
    const int indicesSize = 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1);

    std::vector<float> vertices(verticesSize);
    std::vector<float> normals(normalsSize);
    std::vector<float> textureCoords(texCoordsSize);
    std::vector<int> indices(indicesSize);

    int vertexPointer = 0;
    for (int i = 0; i < VERTEX_COUNT; ++i)
    {
        for (int j = 0; j < VERTEX_COUNT; ++j)
        {
            const float x = -(float)j / ((float)VERTEX_COUNT - 1) * SIZE;
            const float z = -(float)i / ((float)VERTEX_COUNT - 1) * SIZE;

            vertices[vertexPointer * 3] = x;
            vertices[vertexPointer * 3 + 1] = 0.0f;
            vertices[vertexPointer * 3 + 2] = z;

            normals[vertexPointer * 3] = 0.0f;
            normals[vertexPointer * 3 + 1] = 1.0f;
            normals[vertexPointer * 3 + 2] = 0.0f;

            textureCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
            textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);

            ++vertexPointer;
        }
    }

    int pointer = 0;
    for (int gz = 0; gz < VERTEX_COUNT - 1; ++gz)
    {
        for (int gx = 0; gx < VERTEX_COUNT - 1; ++gx)
        {
            const int topLeft = gz * VERTEX_COUNT + gx;
            const int topRight = topLeft + 1;
            const int bottomLeft = (gz + 1) * VERTEX_COUNT + gx;
            const int bottomRight = bottomLeft + 1;

            indices[pointer++] = topLeft;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = topRight;
            indices[pointer++] = topRight;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = bottomRight;
        }
    }

    return loader->LoadData(vertices, textureCoords, normals, indices);
}