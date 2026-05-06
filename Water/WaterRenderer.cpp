#include "StdAfx.h"
#include "WaterRenderer.h"
#include "WaterShader.h"
#include "Loader.h"
#include "WaterTile.h"
#include "Maths.h"
#include "RawModel.h"
#include "WaterFrameBuffers.h"

WaterRenderer::WaterRenderer(Loader* loader, WaterShader* shader, glm::mat4 projectionMatrix, WaterFrameBuffers* frameBuffers)
{
	this->fbo = frameBuffers;
	this->shader = shader;
	shader->Start();
	shader->ConnectTextureUnits();
	shader->LoadProjectionMatrix(projectionMatrix);
	shader->Stop();
	setUpVAO(loader);
}

void WaterRenderer::RenderModel(std::vector<WaterTile*> water, Camera* camera) 
{
	prepareRender(camera);	
	for (WaterTile* tile : water) 
	{
		glm::mat4 modelMatrix = Maths::CreateTransformationMatrix(glm::vec3(tile->GetX(), tile->GetHeight(), tile->GetZ()), glm::vec3(0, 0, 0), WaterTile::TILE_SIZE);
		shader->LoadModelMatrix(modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, quad->GetVertexCount());
	}
	unbind();
}
	
void WaterRenderer::prepareRender(Camera* camera)
{
	shader->Start();
	shader->LoadViewMatrix(camera);
	glBindVertexArray(quad->GetVAOID());
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->GetReflectionTexture());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fbo->GetRefractionTexture());
}
	
void WaterRenderer::unbind()
{
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader->Stop();
}

void WaterRenderer::setUpVAO(Loader* loader) 
{
	// Just x and z vectex positions here, y is set to 0 in v.shader
	std::vector<float> vertices = { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 };
	quad = loader->LoadData(vertices, 2);
}

