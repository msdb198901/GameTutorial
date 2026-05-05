#pragma once

#include <vector>
class RawModel;
class WaterShader;
class Loader;
class WaterTile;
class Camera;

class WaterRenderer {

public:
	WaterRenderer(Loader* loader, WaterShader* shader, glm::mat4 projectionMatrix);

	void RenderModel(std::vector<WaterTile*> water, Camera* camera);

	void CleanUp() {}
private:
	void prepareRender(Camera* camera);
	void unbind();
	void setUpVAO(Loader* loader);

	RawModel* quad;
    WaterShader* shader;
};
