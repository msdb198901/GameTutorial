#pragma once

#include <map>
#include <list>

class TextureModel;
class Light;
class Camera;

class Entity;
class StaticShader;
class EntityRender;

class Terrain;
class TerrainShader;
class TerrainRender;

class EmissiveEntity;
class EmissiveShader;
class EmissiveRender;

// 负责管理所有渲染代码
class MasterRender
{
public:
	MasterRender();
	~MasterRender();

	// 每一帧调用一次
	void Prepare();

	void RenderModel(std::vector<Light*>, Camera*);

	void ProcessEntity(Entity*);

	void ProcessTerrain(Terrain*);

	void ProcessEmissiveEntity(EmissiveEntity*);

	void CleanUp();

public:
	static void EnableCulling();
	static void DisableCulling();

private:
	static const float FOV;
	static const float NEAR_PLANE;
	static const float FAR_PLANE;
	static const float RED;
	static const float GREEN;
	static const float BLUE;

	StaticShader* m_pEntityShader;
	EntityRender* m_pEntityRender;

	TerrainShader* m_pTerrainShader;
	TerrainRender* m_pTerrainRender;

	EmissiveShader* m_pEmissiveShader;
	EmissiveRender* m_pEmissiveRender;

	std::map<TextureModel*, std::vector<Entity*> > m_entities;
	std::list<Terrain*> m_terrains;
	std::list<EmissiveEntity*> m_emissiveEntities;

	glm::mat4 m_projectionMatrix;
};