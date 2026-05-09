#pragma once

#include "ShaderProgram.h"

class Camera;
class WaterShader : public ShaderProgram
{
public:
	WaterShader();
	~WaterShader();

	void BindAttributes() {
		BindAttribute(0, "position");
	}

	void GetAllUniformLocations() {
		location_projectionMatrix = GetUniformLocation("projectionMatrix");
		location_viewMatrix = GetUniformLocation("viewMatrix");
		location_modelMatrix = GetUniformLocation("modelMatrix");
		location_reflectionTexture = GetUniformLocation("reflectionTexture");
		location_refractionTexture = GetUniformLocation("refractionTexture");
		location_dudvMap = GetUniformLocation("dudvMap");
		location_moveFactor = GetUniformLocation("moveFactor");
	}

	void LoadProjectionMatrix(glm::mat4 projection) {
		LoadMatrix(location_projectionMatrix, projection);
	}

	void LoadViewMatrix(Camera* camera);
	void LoadModelMatrix(glm::mat4 modelMatrix);

	void LoadMoveFactor(float move_factor);

	void ConnectTextureUnits();

private:
	int location_modelMatrix;
	int location_viewMatrix;
	int location_projectionMatrix;
	int location_reflectionTexture;
	int location_refractionTexture;
	int location_dudvMap;
	int location_moveFactor;
};
