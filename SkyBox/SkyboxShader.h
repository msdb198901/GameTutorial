#pragma once

#include "ShaderProgram.h"

class Camera;
class SkyboxShader : public ShaderProgram {
public:
	SkyboxShader();

	void LoadProjectionMatrix(glm::mat4 matrix);

	void LoadViewMatrix(Camera* camera);

	void LoadFogColor(glm::vec3 color);

	void ConnectTextureUnits();

	void LoadBlendFactor(float factor);

protected:
	void GetAllUniformLocations();

	void BindAttributes();

private:
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_fogColor;

	int location_cubeMap;
	int location_cubeMap2;
	int location_blendFactor;
};
