#include "StdAfx.h"
#include "StaticShader.h"
#include "Maths.h"
#include "Camera.h"
#include "Light.h"

const char* vertexFile = "E:\\Learn\\OpenGL\\GameTutorial\\Shaders\\shader.vs";
const char* fragmentFile = "E:\\Learn\\OpenGL\\GameTutorial\\Shaders\\shader.fs";

StaticShader::StaticShader()
{
	LoadShader(vertexFile, fragmentFile);
}

void StaticShader::BindAttributes()
{
	BindAttribute(0, "position");
	BindAttribute(1, "textureCoords");
	BindAttribute(2, "normal");
}

void StaticShader::LoadTransformationMatrix(const glm::mat4& transformationMatrix)
{
	//std::cout << "Transformation Matrix: \n" << transformationMatrix[0][0] << std::endl;
	LoadMatrix(m_location_transformationMatrix, transformationMatrix);
}

void StaticShader::LoadViewMatrix(Camera *camera)
{
	glm::mat4 viewMatrix = Maths::CreateViewMatrix(camera);
	LoadMatrix(m_location_viewMatrix, viewMatrix);
}

void StaticShader::LoadProjectionMatrix(const glm::mat4& projectionMatrix)
{
	LoadMatrix(m_location_projectionMatrix, projectionMatrix);
}

void StaticShader::LoadLight(Light* light)
{
	LoadVector(m_location_lightPosition, light->GetPosition());
	LoadVector(m_location_lightColor, light->GetColor());
}

void StaticShader::LoadShineVariables(float damper, float reflectivity)
{
	LoadFloat(m_location_shineDamper, damper);
	LoadFloat(m_location_reflectivity, reflectivity);
}

void StaticShader::LoadFakeLightingVariable(bool useFake)
{
	LoadFloat(m_location_useFakeLighting, useFake);
}

void StaticShader::LoadSkyColor(const glm::vec3& skyColor)
{
	LoadVector(m_location_skyColor, skyColor);
}

void StaticShader::GetAllUniformLocations()
{
	m_location_transformationMatrix = GetUniformLocation("transformationMatrix");
	m_location_viewMatrix = GetUniformLocation("viewMatrix");
	m_location_projectionMatrix = GetUniformLocation("projectionMatrix");
	m_location_lightPosition = GetUniformLocation("lightPosition");
	m_location_lightColor = GetUniformLocation("lightColor");
	m_location_shineDamper = GetUniformLocation("shineDamper");
	m_location_reflectivity = GetUniformLocation("reflectivity");
	m_location_useFakeLighting = GetUniformLocation("useFakeLighting");
	m_location_skyColor = GetUniformLocation("skyColor");
}
