#pragma once

#include <list>
class RawModel;
class Loader;
class GuiTexture;
class GuiShader;
class GuiRender 
{
public:
	GuiRender(Loader *loader);
	~GuiRender();

	void RenderModel(std::list<GuiTexture*> textures);
	void CleanUp();

private:
	// 四边形模型  x y 
	RawModel *quad;
	GuiShader *shader;
};