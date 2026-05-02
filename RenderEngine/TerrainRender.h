#pragma once

#include <list>

class Terrain;
class TerrainShader;
// 渲染类  开始 渲染VAO Loader加载类
class TerrainRender
{
public:
	TerrainRender(TerrainShader*shader, glm::mat4 projectionMatrix);
	~TerrainRender();

	void RenderModel(std::list<Terrain*>& terrains);

	void CleanUp();

	void PrepareTerrain(Terrain* terrain);
	void UnBindTextureModel();
	void LoadModelMatrix(Terrain *terrain);

private:
	TerrainShader *m_shader;
};