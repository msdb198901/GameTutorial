#include "StdAfx.h"
#include "TerrainRender.h"
#include "RawModel.h"
#include "TextureModel.h"
#include "Texture.h"
#include "Terrain.h"
#include "Maths.h"
#include "TerrainShader.h"
#include "DisplayManager.h"

TerrainRender::TerrainRender(TerrainShader* shader, glm::mat4 projectionMatrix)
{
	m_shader = shader;
	shader->Start();
	shader->LoadProjectionMatrix(projectionMatrix);
	shader->Stop();
}

TerrainRender::~TerrainRender()
{
}

void TerrainRender::RenderModel(std::list<Terrain*>& entities)
{
	for (auto& pair : entities)
	{
		PrepareTerrain(pair);
		LoadModelMatrix(pair);
		glDrawElements(GL_TRIANGLES, pair->GetRawModel()->GetVertexCount(), GL_UNSIGNED_INT, 0);
		UnBindTextureModel();
	}
}

void TerrainRender::PrepareTerrain(Terrain* terrain)
{
	RawModel* model = terrain->GetRawModel();

	glBindVertexArray(model->GetVAOID());

	// 激动存储数据的数量列表 0 位置 存储的顶点数据
	glEnableVertexAttribArray(0);

	glEnableVertexAttribArray(1);

	glEnableVertexAttribArray(2);

	Texture* texture = terrain->GetTexture();
	m_shader->LoadShineVariables(texture->GetShineDamper(), texture->GetReflectivity());

	// 告诉OpenGL要把哪个纹理激活（渲染到四边形上）
	// 激活第一个纹理单元 GL_TEXTURE0 
	// Sampler2D默认使用GL_TEXTURE0中的纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetID());
}

void TerrainRender::UnBindTextureModel()
{
	// 绘画完成后，需要禁用属性列表 这里是 0 位置属性列表
	glDisableVertexAttribArray(0);

	glDisableVertexAttribArray(1);

	glDisableVertexAttribArray(2);

	// 解绑顶点数组对象
	glBindVertexArray(0);
}

void TerrainRender::LoadModelMatrix(Terrain* terrain)
{
	glm::mat4 transformationMartix = Maths::CreateTransformationMatrix(glm::vec3(terrain->GetX(), 0, terrain->GetZ()), glm::vec3(0,0,0), 1.0);
	m_shader->LoadTransformationMatrix(transformationMartix);
}

void TerrainRender::CleanUp()
{
}