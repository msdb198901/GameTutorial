#include "StdAfx.h"
#include "ObjLoader.h"
#include <fstream>
#include <sstream>
#include "Loader.h"

RawModel* ObjLoader::LoadObjModel(const char* filePath, Loader* loader)
{
    std::ifstream file(filePath, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filePath << std::endl;
        return nullptr;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<Vertex> vertices;
    std::string line;

    try
    {
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {  // 顶点位置
                glm::vec3 pos;
                iss >> pos.x >> pos.y >> pos.z;
                positions.push_back(pos);
            }
            else if (prefix == "vt") {  // 纹理坐标
                glm::vec2 uv;
                iss >> uv.x >> uv.y;
                texCoords.push_back(uv);
            }
            else if (prefix == "vn") {  // 顶点法线
                glm::vec3 normal;
                iss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
            else if (prefix == "f") {
                std::string v1, v2, v3;
                iss >> v1 >> v2 >> v3;
                std::string verts[3] = { v1, v2, v3 };

                for (int i = 0; i < 3; ++i) {
                    std::istringstream viss(verts[i]);
                    std::string posStr, texStr, normStr;
                    std::getline(viss, posStr, '/');
                    std::getline(viss, texStr, '/');
                    std::getline(viss, normStr, '/');

                    int posIdx = std::stoi(posStr) - 1;
                    int texIdx = texStr.empty() ? -1 : std::stoi(texStr) - 1;
                    int normIdx = normStr.empty() ? -1 : std::stoi(normStr) - 1;

                    Vertex vert;
                    vert.position = positions[posIdx];
                    if (texIdx >= 0 && texIdx < (int)texCoords.size()) {
                        vert.texCoords = texCoords[texIdx];
                        vert.texCoords.y = 1.0f - vert.texCoords.y;
                    }
                    if (normIdx >= 0 && normIdx < (int)normals.size()) {
                        vert.normal = normals[normIdx];
                    }
                    vertices.push_back(vert);
                    indices.push_back(vertices.size() - 1);  // 当前顶点在 vertices 中的索引
                }
            }
        }
        file.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "加载模型出错: " << filePath << " error:" << e.what() << std::endl;
        return nullptr;
    }

    if (vertices.empty()) {
        std::cerr << "加载模型出错: " << filePath << " 没有顶点数据" << std::endl;
        return nullptr;
    }

    std::vector<float> vPositions;
    std::vector<float> vTextureCoords;
    std::vector<float> vNormals;
    std::vector<int> vIndices;
    vPositions.resize(vertices.size() * 3);
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        vPositions[i * 3] = vertices[i].position.x;
        vPositions[i * 3 + 1] = vertices[i].position.y;
        vPositions[i * 3 + 2] = vertices[i].position.z;
    }

    vTextureCoords.resize(vertices.size() * 2);
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        vTextureCoords[i * 2] = vertices[i].texCoords.x;
        vTextureCoords[i * 2 + 1] = vertices[i].texCoords.y;
    }

    vNormals.resize(vertices.size() * 3);
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        vNormals[i * 3] = vertices[i].normal.x;
        vNormals[i * 3 + 1] = vertices[i].normal.y;
        vNormals[i * 3 + 2] = vertices[i].normal.z;
    }

    vIndices.resize(indices.size());
    for (size_t i = 0; i < indices.size(); ++i)
    {
        vIndices[i] = indices[i];
    }
    RawModel* model = loader->LoadData(vPositions, vTextureCoords, vNormals, vIndices);
    return model;
}

