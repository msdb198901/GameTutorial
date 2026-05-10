#pragma once

#include <map>
#include <list>

class TextureModel;
class Light;
class Camera;
class Loader;

class Entity;
class StaticShader;
class EntityRender;

class Terrain;
class TerrainShader;
class TerrainRender;

class EmissiveEntity;
class EmissiveShader;
class EmissiveRender;

class AnimatedRender;
class AnimatedEntity;

class SkyboxRender;

// 负责管理所有渲染代码
class MasterRender
{
public:
	MasterRender(Loader *loader);
	~MasterRender();

	// 每一帧调用一次
	void Prepare();

	void RenderModel(std::vector<Light*>, Camera*, glm::vec4);

	void RenderScene(std::vector<Entity*>, std::vector<AnimatedEntity*>, std::vector<EmissiveEntity*>, std::vector<Terrain*>, std::vector<Light*>, Camera*, glm::vec4);

	void ProcessEntity(Entity*);

	void ProcessTerrain(Terrain*);

	void ProcessEmissiveEntity(EmissiveEntity*);

	void ProcessAnimatedEntity(AnimatedEntity* entity);

	void CleanUp();

	glm::vec3 GetFogColor();

	glm::mat4 GetProjectionMatrix();

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

	static const float NIGHT_RED;
	static const float NIGHT_GREEN;
	static const float NIGHT_BLUE;

	StaticShader* m_pEntityShader;
	EntityRender* m_pEntityRender;

	TerrainShader* m_pTerrainShader;
	TerrainRender* m_pTerrainRender;

	EmissiveShader* m_pEmissiveShader;
	EmissiveRender* m_pEmissiveRender;

	SkyboxRender* m_pSkyboxRender;

	std::map<TextureModel*, std::vector<Entity*> > m_entities;
	std::list<Terrain*> m_terrains;
	std::list<EmissiveEntity*> m_emissiveEntities;

	AnimatedRender* m_pAnimatedRender;
	std::vector<AnimatedEntity*> m_animatedEntities;

	glm::mat4 m_projectionMatrix;
};