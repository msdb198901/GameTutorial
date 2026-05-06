#include "StdAfx.h"
#include "GuiShader.h"

const char* GuiShader::VERTEX_FILE = "Guis\\GuiVertexShader.vs";
const char* GuiShader::FRAGMENT_FILE = "Guis\\GuiFragmentShader.fs";

GuiShader::GuiShader() {
	LoadShader(GuiShader::VERTEX_FILE, GuiShader::FRAGMENT_FILE);
}

GuiShader::~GuiShader() {
}

void GuiShader::LoadTransformation(glm::mat4 matrix) {
	LoadMatrix(location_transformationMatrix, matrix);
}