#include "StdAfx.h"
#include "EntityRender.h"
#include "RawModel.h"
#include "TextureModel.h"
#include "Texture.h"
#include "Entity.h"
#include "Maths.h"
#include "StaticShader.h"
#include "DisplayManager.h"
#include "MasterRender.h"

EntityRender::EntityRender(StaticShader* shader, glm::mat4 projectionMatrix)
{
	m_shader = shader;
	shader->Start();
	shader->LoadProjectionMatrix(projectionMatrix);
	shader->Stop();
}

EntityRender::~EntityRender()
{
}

void EntityRender::RenderModel(Entity* entity, StaticShader* shader)
{
	TextureModel *textureModel = entity->GetTextureModel();

	RawModel *model = textureModel->GetRawModel();

	glBindVertexArray(model->GetVAOID());

	// 激动存储数据的数量列表 0 位置 存储的顶点数据
	glEnableVertexAttribArray(0);

	glEnableVertexAttribArray(1);

	glEnableVertexAttribArray(2);

	glm::mat4 transformationMartix = Maths::CreateTransformationMatrix(entity->GetPosition(), entity->GetRotation(), entity->GetScale());
	shader->LoadTransformationMatrix(transformationMartix);

	Texture *texture = textureModel->GetTexture();
	shader->LoadShineVariables(texture->GetShineDamper(), texture->GetReflectivity());

	// 告诉OpenGL要把哪个纹理激活（渲染到四边形上）
	// 激活第一个纹理单元 GL_TEXTURE0 
	// Sampler2D默认使用GL_TEXTURE0中的纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureModel->GetTexture()->GetID());
	glDrawElements(GL_TRIANGLES, model->GetVertexCount(), GL_UNSIGNED_INT, 0);

	// 绘画完成后，需要禁用属性列表 这里是 0 位置属性列表
	glDisableVertexAttribArray(0);

	glDisableVertexAttribArray(1);

	glDisableVertexAttribArray(2);

	// 解绑顶点数组对象
	glBindVertexArray(0);
}

void EntityRender::RenderModel(std::map<TextureModel*, std::vector<Entity*>>& entities)
{
	for (auto& pair : entities)
	{
		PrepareTextureModel(pair.first);
		for (auto& entity : pair.second)
		{
			PrepareInstance(entity);
			glDrawElements(GL_TRIANGLES, pair.first->GetRawModel()->GetVertexCount(), GL_UNSIGNED_INT, 0);
		}
		UnBindTextureModel();
	}
}

void EntityRender::PrepareTextureModel(TextureModel* textureModel)
{
	RawModel* model = textureModel->GetRawModel();

	glBindVertexArray(model->GetVAOID());

	// 激动存储数据的数量列表 0 位置 存储的顶点数据
	glEnableVertexAttribArray(0);

	glEnableVertexAttribArray(1);

	glEnableVertexAttribArray(2);

	Texture* texture = textureModel->GetTexture();
	m_shader->LoadNumberOfRows(texture->GetNumberOfRows());

	if (texture->IsTransparency())
	{
		// 如果物体透明，禁用背面剔除
		MasterRender::DisableCulling();
	}
	m_shader->LoadShineVariables(texture->GetShineDamper(), texture->GetReflectivity());
	m_shader->LoadFakeLightingVariable(texture->IsUseFakeLighting());
	// 告诉OpenGL要把哪个纹理激活（渲染到四边形上）
	// 激活第一个纹理单元 GL_TEXTURE0 
	// Sampler2D默认使用GL_TEXTURE0中的纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureModel->GetTexture()->GetID());
}

void EntityRender::UnBindTextureModel()
{
	// 重新启用背面剔除
	MasterRender::EnableCulling();

	// 绘画完成后，需要禁用属性列表 这里是 0 位置属性列表
	glDisableVertexAttribArray(0);

	glDisableVertexAttribArray(1);

	glDisableVertexAttribArray(2);

	// 解绑顶点数组对象
	glBindVertexArray(0);
}
void EntityRender::PrepareInstance(Entity* entity)
{
	glm::mat4 transformationMartix = Maths::CreateTransformationMatrix(entity->GetPosition(), entity->GetRotation(), entity->GetScale());
	m_shader->LoadTransformationMatrix(transformationMartix);
	m_shader->LoadOffset(entity->GetTextureXOffset(), entity->GetTextureYOffset());
}

void EntityRender::CleanUp()
{
}