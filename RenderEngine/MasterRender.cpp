#include "StdAfx.h"
#include "MasterRender.h"
#include "StaticShader.h"
#include "Render.h"
#include "Entity.h"
#include "TextureModel.h"

MasterRender::MasterRender()
{
	m_pShader = new StaticShader();
	m_pRender = new Render(m_pShader);
}

MasterRender::~MasterRender()
{

}

void MasterRender::RenderModel(Light* pLight, Camera* pCamera)
{ 
	m_pRender->Prepare();
	m_pShader->Start();

	m_pShader->LoadLight(pLight);

	m_pShader->LoadViewMatrix(pCamera);

	m_pRender->RenderModel(m_entities);

	m_pShader->Stop();
	m_entities.clear();
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

void MasterRender::CleanUp()
{
	m_pShader->CleanUp();
}