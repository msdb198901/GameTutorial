#pragma once

#include "ShaderProgram.h"

class GuiShader : public ShaderProgram
{
public:
	GuiShader();
	~GuiShader();

	void LoadTransformation(glm::mat4 matrix);

	void GetAllUniformLocations() {
		location_transformationMatrix = GetUniformLocation("transformationMatrix");
	}

	void BindAttributes() {
		BindAttribute(0, "position");
	}

private:
	static const char* VERTEX_FILE;
	static const char* FRAGMENT_FILE;
	int location_transformationMatrix;
};