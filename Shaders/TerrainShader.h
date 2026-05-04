#pragma once

#include "ShaderProgram.h"

class Camera;
class Light;
class TerrainShader : public ShaderProgram
{
public:
	TerrainShader();

	virtual void BindAttributes();

	virtual void GetAllUniformLocations();

	void LoadTransformationMatrix(const glm::mat4& transformationMatrix);
	void LoadViewMatrix(Camera* camera);
	void LoadProjectionMatrix(const glm::mat4& projectionMatrix);

	void LoadLights(std::vector<Light*> lights);

	void LoadShineVariables(float shineDamper, float reflectivity);

	void LoadSkyColor(const glm::vec3& color);

	void ConnectTextureUnits();
private:
	int m_location_transformationMatrix;
	int m_location_viewMatrix;
	int m_location_projectionMatrix;
	int m_location_shineDamper;
	int m_location_reflectivity;
	int m_location_skyColor;
	int m_location_backgroundTexture;
	int m_location_rTexture;
	int m_location_gTexture;
	int m_location_bTexture;
	int m_location_blendMap;

	int m_location_lightPosition[MAX_LIGHTS];
	int m_location_lightColor[MAX_LIGHTS];
	int m_location_attenuation[MAX_LIGHTS];
};