#pragma once

#include <vector>
class RawModel;
class WaterShader;
class Loader;
class WaterTile;
class Camera;
class Light;
class WaterFrameBuffers;

class WaterRenderer {

public:
	WaterRenderer(Loader* loader, WaterShader* shader, glm::mat4 projectionMatrix, WaterFrameBuffers* buffers);

	void RenderModel(std::vector<WaterTile*> water, Camera* camera, Light* light);

	void CleanUp() {}
private:
	void prepareRender(Camera* camera, Light* light);
	void unbind();
	void setUpVAO(Loader* loader);

	RawModel* quad;
    WaterShader* shader;
	WaterFrameBuffers* fbo;
	int dudvTextureID;
	int normalTextureID;
	float moveFactor;
	long long lastFrameTime;
};
