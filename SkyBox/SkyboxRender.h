#pragma once

class RawModel;
class SkyboxShader;
class Camera;
class Loader;

class SkyboxRender {
public:
	SkyboxRender(Loader* loader, glm::mat4 projection);
	~SkyboxRender();

	void RenderModel(Camera* camera, glm::vec3 fogColor);

private:
	void BindTextures();

	RawModel* cube;
	int textureID;
	int textureIDNight;
	SkyboxShader* shader;
};
