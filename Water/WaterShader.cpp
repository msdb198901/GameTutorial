#include "StdAfx.h"
#include "WaterShader.h"
#include "Maths.h"

const char* VERTEX_FILE = "Water\\water.vs";
const char* FRAGMENT_FILE = "Water\\water.fs";

WaterShader::WaterShader()
{
	LoadShader(VERTEX_FILE, FRAGMENT_FILE);
}

WaterShader::~WaterShader()
{

}

void WaterShader::LoadViewMatrix(Camera* camera) {
	glm::mat4 viewMatrix = Maths::CreateViewMatrix(camera);
	LoadMatrix(location_viewMatrix, viewMatrix);
}

void WaterShader::LoadMoveFactor(float move_factor)
{
	LoadFloat(location_moveFactor, move_factor);
}

void WaterShader::LoadModelMatrix(glm::mat4 modelMatrix)
{
	LoadMatrix(location_modelMatrix, modelMatrix);
}

void WaterShader::ConnectTextureUnits()
{
	LoadInt(location_reflectionTexture, 0);
	LoadInt(location_refractionTexture, 1);
	LoadInt(location_dudvMap, 2);
}