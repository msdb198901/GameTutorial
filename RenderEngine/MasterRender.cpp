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

MasterRender::MasterRender()
{
	static const float FOV = 70;
	static const float NEAR_PLANE = 0.1f;
	static const float FAR_PLANE = 1000.0f;
	m_projectionMatrix = Maths::CreateProjectionMatrix(FOV, DisplayManager::WIDTH / DisplayManager::HEIHGT, NEAR_PLANE, FAR_PLANE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_pEntityShader = new StaticShader();
	m_pEntityRender = new EntityRender(m_pEntityShader, m_projectionMatrix);

	m_pTerrainShader = new TerrainShader();
	m_pTerrainRender = new TerrainRender(m_pTerrainShader, m_projectionMatrix);
}

MasterRender::~MasterRender()
{

}

void MasterRender::Prepare()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.42, 0.86, 0.83, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRender::RenderModel(Light* pLight, Camera* pCamera)
{ 
	Prepare();
	m_pEntityShader->Start();
	m_pEntityShader->LoadLight(pLight);
	m_pEntityShader->LoadViewMatrix(pCamera);
	m_pEntityRender->RenderModel(m_entities);
	m_pEntityShader->Stop();

	m_pTerrainShader->Start();
	m_pTerrainShader->LoadLight(pLight);
	m_pTerrainShader->LoadViewMatrix(pCamera);
	m_pTerrainRender->RenderModel(m_terrains);
	m_pTerrainShader->Stop();

	m_entities.clear();
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

void MasterRender::CleanUp()
{
	m_pEntityShader->CleanUp();
	m_pTerrainShader->CleanUp();
}