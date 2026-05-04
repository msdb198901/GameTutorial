#pragma once

class RawModel;
//模型数据加载到VBO中
class Loader
{
public:
	// 加载顶点数据 返回一个包含VAO信息的原始模型对象
	RawModel* LoadData(std::vector<float> positions, std::vector<float> vTextureCoords, std::vector<float> vNormals, std::vector<int> vIndices);

	RawModel* LoadData(std::vector<float> positions);

	int CreateVAO();

	void StoreDataInAttributeList(int iVAO, int coordinateSize, std::vector<float> vPositions);

	void BindIndicesBuffer(std::vector<int> vIndices);

	void ClearUp();

	int LoadTexture(const char* fileName);

private :
	void unbindVAO();

private:
	std::vector<GLuint> m_vVAO;
	std::vector<GLuint> m_vVBO;
	std::vector<GLuint> m_vEBO;
	std::vector<GLuint> m_vTextures;
};