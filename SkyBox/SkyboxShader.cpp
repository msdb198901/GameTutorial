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
	LoadMatrix(location_viewMatrix, matrix);
}

void SkyboxShader::GetAllUniformLocations() {
	location_projectionMatrix = GetUniformLocation("projectionMatrix");
	location_viewMatrix = GetUniformLocation("viewMatrix");
}

void SkyboxShader::BindAttributes() 
{
	BindAttribute(0, "position");
}

