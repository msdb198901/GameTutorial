#pragma once

#include <map>
class Entity;
class StaticShader;
class TextureModel;
// 渲染类  开始 渲染VAO Loader加载类
class Render
{
public:
	Render(StaticShader *shader);
	~Render();

	// 每一帧调用一次
	void Prepare();

	// 渲染模型 传入实体和shader 
	// 这样就能将实体的变换参数上传 在不同位置渲染纹理模型
	void RenderModel(Entity *entity, StaticShader *shader);

	void RenderModel(std::map<TextureModel*, std::vector<Entity*>>& entities);

	void CleanUp();

	void PrepareTextureModel(TextureModel *textureModel);
	void UnBindTextureModel();
	void PrepareInstance(Entity *entity);

private:
	glm::mat4 m_projectionMatrix;
	static const float FOV;
	static const float NEAR_PLANE;
	static const float FAR_PLANE;
	StaticShader *m_shader;
};