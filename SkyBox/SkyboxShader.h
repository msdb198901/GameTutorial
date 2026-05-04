#pragma once

#include "ShaderProgram.h"

class Camera;
class SkyboxShader : public ShaderProgram {
public:
	SkyboxShader();

	void LoadProjectionMatrix(glm::mat4 matrix);

	void LoadViewMatrix(Camera* camera);

protected:
	void GetAllUniformLocations();

	void BindAttributes();

private:
	int location_projectionMatrix;
	int location_viewMatrix;
};
