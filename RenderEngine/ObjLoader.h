#pragma once

class Loader;
class RawModel;

// 顶点结构体，包含位置、纹理坐标和法线
struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
};

class ObjLoader
{
public:
	static RawModel *LoadObjModel(const char* fileName, Loader *loader);
};