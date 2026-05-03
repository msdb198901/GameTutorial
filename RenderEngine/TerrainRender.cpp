#include "StdAfx.h"
#include "TerrainRender.h"
#include "RawModel.h"
#include "TextureModel.h"
#include "Texture.h"
#include "Terrain.h"
#include "Maths.h"
#include "TerrainShader.h"
#include "DisplayManager.h"
#include "TerrainTexturePack.h"
#include "TerrainTexture.h"

using namespace std;

TerrainRender::TerrainRender(TerrainShader* shader, glm::mat4 projectionMatrix)
{
	m_shader = shader;
	shader->Start();
	shader->LoadProjectionMatrix(projectionMatrix);
	shader->ConnectTextureUnits();
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

	BindTextures(terrain);
	// 处理地形高光 需要设置高光变量 暂时设置为1 0
	m_shader->LoadShineVariables(1, 0);
}

void TerrainRender::BindTextures(Terrain* terrain)
{
	TerrainTexturePack* texture = terrain->GetTexturePack();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetBackgroundTexture()->GetTextureID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture->GetRTexture()->GetTextureID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture->GetGTexture()->GetTextureID());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture->GetBTexture()->GetTextureID());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrain->GetBlendMap()->GetTextureID());
}

void TerrainRender::UnbindTextures()
{
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