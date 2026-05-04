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

void MasterRender::RenderModel(std::vector<Light*> lights, Camera* pCamera)
{ 
	Prepare();
	m_pEntityShader->Start();
	m_pEntityShader->LoadSkyColor(glm::vec3(MasterRender::RED, MasterRender::GREEN, MasterRender::BLUE));
	m_pEntityShader->LoadLights(lights);
	m_pEntityShader->LoadViewMatrix(pCamera);
	m_pEntityRender->RenderModel(m_entities);
	m_pEntityShader->Stop();

	m_pEmissiveShader->Start();
	m_pEmissiveShader->LoadViewMatrix(pCamera);
	m_pEmissiveRender->RenderModel(m_emissiveEntities);
	m_pEmissiveShader->Stop();

	m_pTerrainShader->Start();
	m_pTerrainShader->LoadSkyColor(glm::vec3(MasterRender::RED, MasterRender::GREEN, MasterRender::BLUE));
	m_pTerrainShader->LoadLights(lights);
	m_pTerrainShader->LoadViewMatrix(pCamera);
	m_pTerrainRender->RenderModel(m_terrains);
	m_pTerrainShader->Stop();

	m_pSkyboxRender->RenderModel(pCamera);

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