#include "StdAfx.h"
#include "Terrain.h"
#include "Loader.h"
#include "Texture.h"
#include "stb_image.h"

float Terrain::SIZE = 800;

class HeightMap {
private:
    unsigned char* data;
    int width, height;
    float maxHeight;
    float totalWidth;
    float totalHeight;
    static constexpr float MAX_PIXEL_COLOUR = 256.0f * 256.0f * 256.0f; // 16,777,216.0f
    static constexpr float HALF_MAX = MAX_PIXEL_COLOUR / 2.0f;           // 8,388,608.0f

public:
    HeightMap(const char* filename, float heigh) : maxHeight(heigh) {
        int channels;
        // 强制加载为 RGB 3 通道（与 Java 的 getRGB 一致）
        data = stbi_load(filename, &width, &height, &channels, 4);
        if (!data) {
            throw std::runtime_error("Failed to load heightmap");
        }
        printf("Loaded RGB heightmap: %dx%d\n", width, height);
    }

    ~HeightMap() {
        stbi_image_free(data);
    }

    int GetHeight() const {
        return height;
    }

    float GetHeight(int x, int z) const {
        if (x < 0 || x >= width || z < 0 || z >= height) return 0.0f;

        int idx = (z * width + x) * 4;  // RGBA
        // 获取RGB分量，组合成24位整数 rgb (0~16777215)
        int r = data[idx];
        int g = data[idx + 1];
        int b = data[idx + 2];
        int rgb = (r << 16) | (g << 8) | b;

        const float MAX_PIXEL_COLOUR = 256.0f * 256.0f * 256.0f; // 16777216
        const float HALF_MAX = MAX_PIXEL_COLOUR / 2.0f;           // 8388608

        // 注意：Java中 image.getRGB 返回值为 rgb - MAX_PIXEL_COLOUR (因alpha=0xFF)
        // 所以 Java 公式等价于 ((rgb - MAX_PIXEL_COLOUR) + HALF_MAX) / HALF_MAX * MAX_HEIGHT
        // 即 (rgb - HALF_MAX) / HALF_MAX * MAX_HEIGHT
        float height = (static_cast<float>(rgb) - HALF_MAX) / HALF_MAX * maxHeight;
      
        return height;
    }
};

Terrain::Terrain(int gridX, int gridZ, Loader* loader, TerrainTexturePack* texturePack, TerrainTexture* blendMap, const char* heightMapPath)
{
	this->x = gridX * SIZE;
	this->z = gridZ * SIZE;
	this->texturePack = texturePack;
	this->blendMap = blendMap;
	this->model = GenerateTerrain(loader, heightMapPath);
}

Terrain::~Terrain()
{

}


RawModel* Terrain::GenerateTerrain(Loader* loader, const char* heightMapPath)
{
    heightMap = new HeightMap(heightMapPath, 40.0f);

    int VERTEX_COUNT = heightMap->GetHeight();

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
            vertices[vertexPointer * 3 + 1] = heightMap->GetHeight(j, i);
            vertices[vertexPointer * 3 + 2] = z;

            // 计算法线向量
            glm::vec3 normal = CalculateNormal(j, i, heightMap);
            normals[vertexPointer * 3] = normal.x;
            normals[vertexPointer * 3 + 1] = normal.y;
            normals[vertexPointer * 3 + 2] = normal.z;

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

glm::vec3 Terrain::CalculateNormal(int x, int z, HeightMap *heightMap)
{
    float heightL = heightMap->GetHeight(x - 1, z);
    float heightR = heightMap->GetHeight(x + 1, z);
    float heightD = heightMap->GetHeight(x, z - 1);
    float heightU = heightMap->GetHeight(x, z + 1);

    glm::vec3 normal;
    normal.x = heightL - heightR;
    normal.y = 2.0f;
    normal.z = heightD - heightU;

    normal = glm::normalize(normal);
    return normal;
}

