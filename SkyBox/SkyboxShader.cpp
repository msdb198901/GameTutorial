#include "StdAfx.h"
#include "SkyboxShader.h"
#include "Maths.h"

const char* skyboxVertexFile = "E:\\Learn\\OpenGL\\GameTutorial\\SkyBox\\skybox.vs";
const char* skyboxFragmentFile = "E:\\Learn\\OpenGL\\GameTutorial\\SkyBox\\skybox.fs";


SkyboxShader::SkyboxShader() 
{
	LoadShader(skyboxVertexFile, skyboxFragmentFile);
}
	
void SkyboxShader::LoadProjectionMatrix(glm::mat4 matrix)
{
	LoadMatrix(location_projectionMatrix, matrix);
}

void SkyboxShader::LoadViewMatrix(Camera* camera)
{
	glm::mat4 matrix = Maths::CreateViewMatrix(camera);
	// Remove translation from view matrix so that skybox always stays at the center of camera
	matrix = glm::mat4(glm::mat3(matrix));
	// ÈÆ y Öá »ºÂýÐý×ª
	matrix = glm::rotate(matrix, 0.01f * (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

	LoadMatrix(location_viewMatrix, matrix);
}

void SkyboxShader::ConnectTextureUnits()
{
	LoadInt(location_cubeMap, 0);
	LoadInt(location_cubeMap2, 1);
}

void SkyboxShader::LoadFogColor(glm::vec3 color)
{
	LoadVector(location_fogColor, color);
}

void SkyboxShader::LoadBlendFactor(float factor)
{
	LoadFloat(location_blendFactor, factor);
}

void SkyboxShader::GetAllUniformLocations() {
	location_projectionMatrix = GetUniformLocation("projectionMatrix");
	location_viewMatrix = GetUniformLocation("viewMatrix");
	location_fogColor = GetUniformLocation("fogColor");

	location_cubeMap = GetUniformLocation("cubeMap");
	location_cubeMap2 = GetUniformLocation("cubeMap2");
	location_blendFactor = GetUniformLocation("blendFactor");
}

void SkyboxShader::BindAttributes() 
{
	BindAttribute(0, "position");
}

