#include "StdAfx.h"
#include "GuiRender.h"
#include "Loader.h"
#include "RawModel.h"
#include "GuiShader.h"
#include "GuiTexture.h"
#include "Maths.h"

GuiRender::GuiRender(Loader* loader)
{
	std::vector<float> vertices = {-1, 1, -1, -1, 1, 1, 1, -1};
	quad = loader->LoadData(vertices, 2);
	shader = new GuiShader();
}

GuiRender::~GuiRender()
{ }

void GuiRender::RenderModel(std::list<GuiTexture*> textures)
{
	shader->Start();
	glBindVertexArray(quad->GetVAOID());
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	for (GuiTexture* texture : textures)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glm::mat4 model = Maths::CreateTransformationMatrix(texture->GetPosition(), texture->GetScale());
		shader->LoadTransformation(model);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, quad->GetVertexCount());
	}
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader->Stop();
}

void GuiRender::CleanUp()
{
	shader->CleanUp();
	delete quad;
	delete shader;
}