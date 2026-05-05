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
	}

	void LoadProjectionMatrix(glm::mat4 projection) {
		LoadMatrix(location_projectionMatrix, projection);
	}

	void LoadViewMatrix(Camera* camera);
	void LoadModelMatrix(glm::mat4 modelMatrix);

private:
	int location_modelMatrix;
	int location_viewMatrix;
	int location_projectionMatrix;
};
