#pragma once

class RawModel;
class SkyboxShader;
class Camera;
class Loader;

class SkyboxRender {
public:
	SkyboxRender(Loader* loader, glm::mat4 projection);
	~SkyboxRender();

	void RenderModel(Camera* camera);

private:
	RawModel* cube;
	int textureID;
	SkyboxShader* shader;
};
