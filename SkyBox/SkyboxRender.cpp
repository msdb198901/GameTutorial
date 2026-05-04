#include "StdAfx.h"
#include "SkyboxRender.h"
#include "Loader.h"
#include "SkyboxShader.h"
#include "Camera.h"
#include "RawModel.h"

const float SIZE = 500.0f;
const std::vector<float> VERTICES = {
	-SIZE,  SIZE, -SIZE,
	-SIZE, -SIZE, -SIZE,
	 SIZE, -SIZE, -SIZE,
	 SIZE, -SIZE, -SIZE,
	 SIZE,  SIZE, -SIZE,
	-SIZE,  SIZE, -SIZE,

	-SIZE, -SIZE,  SIZE,
	-SIZE, -SIZE, -SIZE,
	-SIZE,  SIZE, -SIZE,
	-SIZE,  SIZE, -SIZE,
	-SIZE,  SIZE,  SIZE,
	-SIZE, -SIZE,  SIZE,

	 SIZE, -SIZE, -SIZE,
	 SIZE, -SIZE,  SIZE,
	 SIZE,  SIZE,  SIZE,
	 SIZE,  SIZE,  SIZE,
	 SIZE,  SIZE, -SIZE,
	 SIZE, -SIZE, -SIZE,

	-SIZE, -SIZE,  SIZE,
	-SIZE,  SIZE,  SIZE,
	 SIZE,  SIZE,  SIZE,
	 SIZE,  SIZE,  SIZE,
	 SIZE, -SIZE,  SIZE,
	-SIZE, -SIZE,  SIZE,

	-SIZE,  SIZE, -SIZE,
	 SIZE,  SIZE, -SIZE,
	 SIZE,  SIZE,  SIZE,
	 SIZE,  SIZE,  SIZE,
	-SIZE,  SIZE,  SIZE,
	-SIZE,  SIZE, -SIZE,

	-SIZE, -SIZE, -SIZE,
	-SIZE, -SIZE,  SIZE,
	 SIZE, -SIZE, -SIZE,
	 SIZE, -SIZE, -SIZE,
	-SIZE, -SIZE,  SIZE,
	 SIZE, -SIZE,  SIZE
};

std::vector<const char*> TextureFiles = {
	"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\right.png",
	"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\left.png",
	"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\top.png",
	"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\bottom.png",
	"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\back.png",
	"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\front.png"
};


SkyboxRender::SkyboxRender(Loader* loader, glm::mat4 projection)
{
	this->cube = loader->LoadData(VERTICES, 3);
	this->textureID = loader->LoadCubeMapTexture(TextureFiles);

	this->shader = new SkyboxShader();
	this->shader->Start();
	this->shader->LoadProjectionMatrix(projection);
	this->shader->Stop();
}

SkyboxRender::~SkyboxRender()
{

}

void SkyboxRender::RenderModel(Camera* camera)
{
	shader->Start();
	shader->LoadViewMatrix(camera);

	glBindVertexArray(cube->GetVAOID());
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, cube->GetVertexCount());

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	shader->Stop();
}