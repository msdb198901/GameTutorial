#include "StdAfx.h"
#include "TerrainShader.h"
#include "Maths.h"
#include "Camera.h"
#include "Light.h"

const char* terrainVertexFile = "E:\\Learn\\OpenGL\\GameTutorial\\Shaders\\terrain.vs";
const char* terrainFragmentFile = "E:\\Learn\\OpenGL\\GameTutorial\\Shaders\\terrain.fs";

TerrainShader::TerrainShader()
{
	LoadShader(terrainVertexFile, terrainFragmentFile);
}

void TerrainShader::BindAttributes()
{
	BindAttribute(0, "position");
	BindAttribute(1, "textureCoords");
	BindAttribute(2, "normal");
}

void TerrainShader::LoadTransformationMatrix(const glm::mat4& transformationMatrix)
{
	//std::cout << "Transformation Matrix: \n" << transformationMatrix[0][0] << std::endl;
	LoadMatrix(m_location_transformationMatrix, transformationMatrix);
}

void TerrainShader::LoadViewMatrix(Camera *camera)
{
	glm::mat4 viewMatrix = Maths::CreateViewMatrix(camera);
	LoadMatrix(m_location_viewMatrix, viewMatrix);
}

void TerrainShader::LoadProjectionMatrix(const glm::mat4& projectionMatrix)
{
	LoadMatrix(m_location_projectionMatrix, projectionMatrix);
}

void TerrainShader::LoadLight(Light* light)
{
	LoadVector(m_location_lightPosition, light->GetPosition());
	LoadVector(m_location_lightColor, light->GetColor());
}

void TerrainShader::LoadShineVariables(float damper, float reflectivity)
{
	LoadFloat(m_location_shineDamper, damper);
	LoadFloat(m_location_reflectivity, reflectivity);
}

void TerrainShader::LoadSkyColor(const glm::vec3& color)
{
	LoadVector(m_location_skyColor, color);
}

void TerrainShader::ConnectTextureUnits()
{
	LoadInt(m_location_backgroundTexture, 0);
	LoadInt(m_location_rTexture, 1);
	LoadInt(m_location_gTexture, 2);
	LoadInt(m_location_bTexture, 3);
	LoadInt(m_location_blendMap, 4);
}

void TerrainShader::GetAllUniformLocations()
{
	m_location_transformationMatrix = GetUniformLocation("transformationMatrix");
	m_location_viewMatrix = GetUniformLocation("viewMatrix");
	m_location_projectionMatrix = GetUniformLocation("projectionMatrix");
	m_location_lightPosition = GetUniformLocation("lightPosition");
	m_location_lightColor = GetUniformLocation("lightColor");
	m_location_shineDamper = GetUniformLocation("shineDamper");
	m_location_reflectivity = GetUniformLocation("reflectivity");
	m_location_skyColor = GetUniformLocation("skyColor");
	m_location_backgroundTexture = GetUniformLocation("backgroundTexture");
	m_location_rTexture = GetUniformLocation("rTexture");
	m_location_gTexture = GetUniformLocation("gTexture");
	m_location_bTexture = GetUniformLocation("bTexture");
	m_location_blendMap = GetUniformLocation("blendMap");
}
