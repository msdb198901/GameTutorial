#include "StdAfx.h"
#include "MasterRender.h"
#include "StaticShader.h"
#include "EntityRender.h"
#include "Entity.h"
#include "TextureModel.h"
#include "Maths.h"
#include "DisplayManager.h"
#include "TerrainShader.h"
#include "TerrainRender.h"

#include "EmissiveEntity.h"
#include "EmissiveShader.h"
#include "EmissiveRender.h"

#include "SkyboxRender.h"

const float MasterRender::FOV = 70;
const float MasterRender::NEAR_PLANE = 0.1f;
const float MasterRender::FAR_PLANE = 1000.0f;

//const float MasterRender::RED = 0.42;
//const float MasterRender::GREEN = 0.86;
//const float MasterRender::BLUE = 0.82;

const float MasterRender::RED = 0.5444;
const float MasterRender::GREEN = 0.62;
const float MasterRender::BLUE = 0.69;

const float MasterRender::NIGHT_RED = 0.0;
const float MasterRender::NIGHT_GREEN = 0.027;
const float MasterRender::NIGHT_BLUE = 0.0588;

MasterRender::MasterRender(Loader* loader)
{
	m_projectionMatrix = Maths::CreateProjectionMatrix(MasterRender::FOV, DisplayManager::WIDTH / DisplayManager::HEIHGT, MasterRender::NEAR_PLANE, MasterRender::FAR_PLANE);

	MasterRender::EnableCulling();

	m_pEntityShader = new StaticShader();
	m_pEntityRender = new EntityRender(m_pEntityShader, m_projectionMatrix);

	m_pTerrainShader = new TerrainShader();
	m_pTerrainRender = new TerrainRender(m_pTerrainShader, m_projectionMatrix);

	m_pEmissiveShader = new EmissiveShader();
	m_pEmissiveRender = new EmissiveRender(m_pEmissiveShader, m_projectionMatrix);

	m_pSkyboxRender = new SkyboxRender(loader, m_projectionMatrix);
}

MasterRender::~MasterRender()
{

}

void MasterRender::EnableCulling()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void MasterRender::DisableCulling()
{
	glDisable(GL_CULL_FACE);
}

void MasterRender::Prepare()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(MasterRender::RED, MasterRender::GREEN, MasterRender::BLUE, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRender::RenderScene(std::vector<Entity*> entities, std::vector<EmissiveEntity*> emissives, std::vector<Terrain*> terrains, std::vector<Light*> lights, Camera* camera, glm::vec4 clipPlane)
{
	for (auto entity : entities)
	{
		ProcessEntity(entity);
	}
	for (auto emissiveEntity : emissives)
	{
		ProcessEmissiveEntity(emissiveEntity);
	}
	for (auto terrain : terrains)
	{
		ProcessTerrain(terrain);
	}
	RenderModel(lights, camera, clipPlane);
}

void MasterRender::RenderModel(std::vector<Light*> lights, Camera* pCamera, glm::vec4 clipPlane)
{ 
	glm::vec3 fogColor = GetFogColor();

	Prepare();
	m_pEntityShader->Start();
	m_pEntityShader->LoadClipPlane(clipPlane);
	m_pEntityShader->LoadSkyColor(fogColor);
	m_pEntityShader->LoadLights(lights);
	m_pEntityShader->LoadViewMatrix(pCamera);
	m_pEntityRender->RenderModel(m_entities);
	m_pEntityShader->Stop();

	m_pEmissiveShader->Start();
	m_pEmissiveShader->LoadViewMatrix(pCamera);
	m_pEmissiveRender->RenderModel(m_emissiveEntities);
	m_pEmissiveShader->Stop();

	m_pTerrainShader->Start();
	m_pTerrainShader->LoadClipPlane(clipPlane);
	m_pTerrainShader->LoadSkyColor(fogColor);
	m_pTerrainShader->LoadLights(lights);
	m_pTerrainShader->LoadViewMatrix(pCamera);
	m_pTerrainRender->RenderModel(m_terrains);
	m_pTerrainShader->Stop();

	m_pSkyboxRender->RenderModel(pCamera, fogColor);

	m_entities.clear();
	m_emissiveEntities.clear();
	m_terrains.clear();
}

void MasterRender::ProcessEntity(Entity* pEntity)
{
	TextureModel *entryModel = pEntity->GetTextureModel();
	if (m_entities.find(entryModel) == m_entities.end())
	{
		m_entities[entryModel] = std::vector<Entity*>();
		m_entities[entryModel].push_back(pEntity);
	} 
	else
	{
		m_entities[entryModel].push_back(pEntity);
	}
}

void MasterRender::ProcessTerrain(Terrain* terrain)
{
	m_terrains.push_back(terrain);
}

void  MasterRender::ProcessEmissiveEntity(EmissiveEntity* emissiveEntity)
{
	m_emissiveEntities.push_back(emissiveEntity);
}

void MasterRender::CleanUp()
{
	m_pEmissiveShader->CleanUp();
	m_pEntityShader->CleanUp();
	m_pTerrainShader->CleanUp();
}

glm::mat4 MasterRender::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

glm::vec3 MasterRender::GetFogColor()
{
	double currentSeconds = glfwGetTime();
	long long timeMs = (long long)(currentSeconds * 1000) % 24000;

	// 定义夜晚和白天雾色（你需要根据自己的场景调整）
	glm::vec3 nightFog = glm::vec3(NIGHT_RED, NIGHT_GREEN, NIGHT_BLUE);
	glm::vec3 dayFog = glm::vec3(RED, GREEN, BLUE);

	float blendFactor;  // 复用相同的混合因子
	int phase;          // 0:全夜, 1:夜->昼, 2:全昼, 3:昼->夜

	if (timeMs >= 0 && timeMs < 5000) {
		phase = 0;
		blendFactor = 0.0f;
	}
	else if (timeMs >= 5000 && timeMs < 8000) {
		phase = 1;
		blendFactor = (float)(timeMs - 5000) / (8000 - 5000);
	}
	else if (timeMs >= 8000 && timeMs < 21000) {
		phase = 2;
		blendFactor = 1.0f;
	}
	else { // 21000 ~ 23999
		phase = 3;
		blendFactor = (float)(timeMs - 21000) / (24000 - 21000);
	}

	switch (phase) {
	case 0: return dayFog;
	case 1: return glm::mix(dayFog, nightFog, blendFactor);
	case 2: return nightFog;
	case 3: return glm::mix(nightFog, dayFog, blendFactor);
	}
	return dayFog; // fallback
}