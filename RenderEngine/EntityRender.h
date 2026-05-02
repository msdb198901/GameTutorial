#pragma once

#include <map>
class Entity;
class StaticShader;
class TextureModel;
// 渲染类  开始 渲染VAO Loader加载类
class EntityRender
{
public:
	EntityRender(StaticShader *shader, glm::mat4 projectionMatrix);
	~EntityRender();

	// 渲染模型 传入实体和shader 
	// 这样就能将实体的变换参数上传 在不同位置渲染纹理模型
	void RenderModel(Entity *entity, StaticShader *shader);

	void RenderModel(std::map<TextureModel*, std::vector<Entity*>>& entities);

	void CleanUp();

	void PrepareTextureModel(TextureModel *textureModel);
	void UnBindTextureModel();
	void PrepareInstance(Entity *entity);

private:
	StaticShader *m_shader;
};