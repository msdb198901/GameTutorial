#include "StdAfx.h"
#include "StaticShader.h"
#include "Maths.h"
#include "Camera.h"
#include "Light.h"
#include <string>

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

void StaticShader::LoadLights(std::vector<Light*> lights)
{
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		if (i < lights.size())
		{
			LoadVector(m_location_lightPosition[i], lights[i]->GetPosition());
			LoadVector(m_location_lightColor[i], lights[i]->GetColor());
			LoadVector(m_location_attenuation[i], lights[i]->GetAttenuation());
		}
		else
		{
			LoadVector(m_location_lightPosition[i], glm::vec3(0, 0, 0));
			LoadVector(m_location_lightColor[i], glm::vec3(0, 0, 0));
			LoadVector(m_location_attenuation[i], glm::vec3(1, 0, 0));
		}
	}
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

void StaticShader::LoadNumberOfRows(int numberOfRows)
{
	LoadFloat(m_location_numberOfRows, numberOfRows);
}

void StaticShader::LoadOffset(float x, float y)
{
	LoadVector(m_location_offset, glm::vec2(x, y));
}

void StaticShader::GetAllUniformLocations()
{
	m_location_transformationMatrix = GetUniformLocation("transformationMatrix");
	m_location_viewMatrix = GetUniformLocation("viewMatrix");
	m_location_projectionMatrix = GetUniformLocation("projectionMatrix");
	m_location_shineDamper = GetUniformLocation("shineDamper");
	m_location_reflectivity = GetUniformLocation("reflectivity");
	m_location_useFakeLighting = GetUniformLocation("useFakeLighting");
	m_location_skyColor = GetUniformLocation("skyColor");
	m_location_numberOfRows = GetUniformLocation("numberOfRows");
	m_location_offset = GetUniformLocation("offset");

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		std::string lightPos = "lightPosition[" + std::to_string(i) + "]";
		m_location_lightPosition[i] = GetUniformLocation(lightPos.c_str());

		std::string lightColor = "lightColor[" + std::to_string(i) + "]";
		m_location_lightColor[i] = GetUniformLocation(lightColor.c_str());

		std::string lightAttenuation = "attenuation[" + std::to_string(i) + "]";
		m_location_attenuation[i] = GetUniformLocation(lightAttenuation.c_str());
	}
}
