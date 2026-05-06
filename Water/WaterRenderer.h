#pragma once

#include <vector>
class RawModel;
class WaterShader;
class Loader;
class WaterTile;
class Camera;
class WaterFrameBuffers;

class WaterRenderer {

public:
	WaterRenderer(Loader* loader, WaterShader* shader, glm::mat4 projectionMatrix, WaterFrameBuffers* buffers);

	void RenderModel(std::vector<WaterTile*> water, Camera* camera);

	void CleanUp() {}
private:
	void prepareRender(Camera* camera);
	void unbind();
	void setUpVAO(Loader* loader);

	RawModel* quad;
    WaterShader* shader;
	WaterFrameBuffers* fbo;
};
