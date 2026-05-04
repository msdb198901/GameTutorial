#include "StdAfx.h"
#include "RawModel.h"
#include "Loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

RawModel* Loader::LoadData(std::vector<float> vPositions, std::vector<float> vTextureCoords, std::vector<float> vNormals,  std::vector<int> vIndices)
{
	int VAO = CreateVAO();
	// 顶点数据存储在VAO属性列表 0 位置
	StoreDataInAttributeList(0, 3, vPositions);
	StoreDataInAttributeList(1, 2, vTextureCoords);
	StoreDataInAttributeList(2, 3, vNormals);
	BindIndicesBuffer(vIndices);
	unbindVAO();
	return new RawModel(VAO, vIndices.size());
}

RawModel* Loader::LoadData(std::vector<float> positions, int dimensions)
{
	int VAO = CreateVAO();
	StoreDataInAttributeList(0, dimensions, positions);
	unbindVAO();
	return new RawModel(VAO, positions.size() / dimensions);
}

int Loader::CreateVAO()
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	m_vVAO.emplace_back(VAO);
	glBindVertexArray(VAO);
	return VAO;
}

void Loader::StoreDataInAttributeList(int VAO, int coordinateSize, std::vector<float> vPositions)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	m_vVBO.emplace_back(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vPositions.size() * sizeof(float), vPositions.data(), GL_STATIC_DRAW);

	// 将VBO存入VAO属性列表中 
	glVertexAttribPointer(VAO, coordinateSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//glVertexAttribPointer(VAO, coordinateSize, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// 解绑当前VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::BindIndicesBuffer(std::vector<int> vIndices)
{
	GLuint EBO;
	glGenBuffers(1, &EBO);
	m_vEBO.emplace_back(EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndices.size() * sizeof(int), vIndices.data(), GL_STATIC_DRAW);
}

void Loader::ClearUp()
{
	for (std::vector<GLuint>::iterator it = m_vTextures.begin(); it != m_vTextures.end(); it++)
	{
		glDeleteTextures(1, &(*it));
	}

	for (std::vector<GLuint>::iterator it = m_vEBO.begin(); it != m_vEBO.end(); it++)
	{
		glDeleteBuffers(1, &(*it));
	}

	for (std::vector<GLuint>::iterator it = m_vVBO.begin(); it != m_vVBO.end(); it++)
	{
		glDeleteBuffers(1, &(*it));
	}

	for (std::vector<GLuint>::iterator it = m_vVAO.begin(); it != m_vVAO.end(); it++)
	{
		glDeleteVertexArrays(1, &(*it));
	}
}

int Loader::LoadTexture(const char* fileName)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(fileName, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << fileName << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

int Loader::LoadCubeMapTexture(std::vector<const char*> textureFiles)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (GLuint i = 0; i < textureFiles.size(); i++)
	{
		unsigned char* data = stbi_load(textureFiles[i], &width, &height, &nrChannels, 3);
		if (data)
		{
			GLenum format = GL_RGB;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << textureFiles[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}

void Loader::unbindVAO()
{
	glBindVertexArray(0);
}

