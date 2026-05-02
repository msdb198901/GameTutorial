#include "StdAfx.h"
#include "Render.h"
#include "RawModel.h"
#include "TextureModel.h"
#include "Texture.h"
#include "Entity.h"
#include "Maths.h"
#include "StaticShader.h"
#include "DisplayManager.h"

Render::Render(StaticShader* shader)
{
	m_shader = shader;

	static const float FOV = 70;
	static const float NEAR_PLANE = 0.1f;
	static const float FAR_PLANE = 1000.0f;
	m_projectionMatrix = Maths::CreateProjectionMatrix(FOV, DisplayManager::WIDTH / DisplayManager::HEIHGT, NEAR_PLANE, FAR_PLANE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	shader->Start();
	shader->LoadProjectionMatrix(m_projectionMatrix);
	shader->Stop();
}

Render::~Render()
{
}

void Render::Prepare()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2, 0.3, 0.3, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::RenderModel(Entity* entity, StaticShader* shader)
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

void Render::RenderModel(std::map<TextureModel*, std::vector<Entity*>>& entities)
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

void Render::PrepareTextureModel(TextureModel* textureModel)
{
	RawModel* model = textureModel->GetRawModel();

	glBindVertexArray(model->GetVAOID());

	// 激动存储数据的数量列表 0 位置 存储的顶点数据
	glEnableVertexAttribArray(0);

	glEnableVertexAttribArray(1);

	glEnableVertexAttribArray(2);

	Texture* texture = textureModel->GetTexture();
	m_shader->LoadShineVariables(texture->GetShineDamper(), texture->GetReflectivity());

	// 告诉OpenGL要把哪个纹理激活（渲染到四边形上）
	// 激活第一个纹理单元 GL_TEXTURE0 
	// Sampler2D默认使用GL_TEXTURE0中的纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureModel->GetTexture()->GetID());
}

void Render::UnBindTextureModel()
{
	// 绘画完成后，需要禁用属性列表 这里是 0 位置属性列表
	glDisableVertexAttribArray(0);

	glDisableVertexAttribArray(1);

	glDisableVertexAttribArray(2);

	// 解绑顶点数组对象
	glBindVertexArray(0);
}
void Render::PrepareInstance(Entity* entity)
{
	glm::mat4 transformationMartix = Maths::CreateTransformationMatrix(entity->GetPosition(), entity->GetRotation(), entity->GetScale());
	m_shader->LoadTransformationMatrix(transformationMartix);
}

void Render::CleanUp()
{
}