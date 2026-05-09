#include "StdAfx.h"
#include "WaterRenderer.h"
#include "WaterShader.h"
#include "Loader.h"
#include "WaterTile.h"
#include "Maths.h"
#include "RawModel.h"
#include "WaterFrameBuffers.h"
#include "Light.h"

const float WAVE_SPEED = 0.03f;

WaterRenderer::WaterRenderer(Loader* loader, WaterShader* shader, glm::mat4 projectionMatrix, WaterFrameBuffers* frameBuffers)
{
	this->fbo = frameBuffers;
	this->shader = shader;
	moveFactor = 0;
	dudvTextureID = loader->LoadTexture("Resources\\waterDUDV.png");
	normalTextureID = loader->LoadTexture("Resources\\matchingNormalMap.png");
	shader->Start();
	shader->ConnectTextureUnits();
	shader->LoadProjectionMatrix(projectionMatrix);
	shader->Stop();
	setUpVAO(loader);
}

void WaterRenderer::RenderModel(std::vector<WaterTile*> water, Camera* camera, Light* light)
{
	prepareRender(camera, light);
	for (WaterTile* tile : water) 
	{
		glm::mat4 modelMatrix = Maths::CreateTransformationMatrix(glm::vec3(tile->GetX(), tile->GetHeight(), tile->GetZ()), glm::vec3(0, 0, 0), WaterTile::TILE_SIZE);
		shader->LoadModelMatrix(modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, quad->GetVertexCount());
	}
	unbind();
}
	
void WaterRenderer::prepareRender(Camera* camera, Light* light)
{
	shader->Start();
	shader->LoadViewMatrix(camera);
	shader->LoadLight(light);

	moveFactor += WAVE_SPEED * 0.01;
	moveFactor = modf(moveFactor, &moveFactor);
	shader->LoadMoveFactor(moveFactor);

	glBindVertexArray(quad->GetVAOID());
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->GetReflectionTexture());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fbo->GetRefractionTexture());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvTextureID);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalTextureID);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, fbo->GetRefractionDepthTexture());

	// 水体边缘的混合效果
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
	
void WaterRenderer::unbind()
{
	glDisable(GL_BLEND);
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

