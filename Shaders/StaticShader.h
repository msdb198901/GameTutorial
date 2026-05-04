#pragma once

#include "ShaderProgram.h"

class Camera;
class Light;
class StaticShader : public ShaderProgram
{
public:
	StaticShader();

	virtual void BindAttributes();

	virtual void GetAllUniformLocations();

	void LoadTransformationMatrix(const glm::mat4& transformationMatrix);
	void LoadViewMatrix(Camera* camera);
	void LoadProjectionMatrix(const glm::mat4& projectionMatrix);

	void LoadLight(Light *light); //overload (for convenience)

	void LoadShineVariables(float shineDamper, float reflectivity);

	void LoadFakeLightingVariable(bool useFake);

	void LoadSkyColor(const glm::vec3& skyColor);

	void LoadNumberOfRows(int numberOfRows);
	void LoadOffset(float x, float y);
private:
	int m_location_transformationMatrix;
	int m_location_viewMatrix;
	int m_location_projectionMatrix;
	int m_location_lightPosition;
	int m_location_lightColor;
	int m_location_shineDamper;
	int m_location_reflectivity;
	int m_location_useFakeLighting;
	int m_location_skyColor;
	int m_location_numberOfRows;
	int m_location_offset;
};