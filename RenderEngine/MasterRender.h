#pragma once

#include <map>
class StaticShader;
class TextureModel;
class Entity;
// 负责管理所有渲染代码
class MasterRender
{
public:
	MasterRender();
	~MasterRender();

	void RenderModel(Light*, Camera*);

	void ProcessEntity(Entity*);

	void CleanUp();

private:
	StaticShader* m_pShader;
	Render* m_pRender;
	std::map<TextureModel*, std::vector<Entity*> > m_entities;
};