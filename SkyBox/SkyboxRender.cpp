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
	"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\nightRight.png",
	"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\nightLeft.png",
	"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\nightTop.png",
	"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\nightBottom.png",
	"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\nightBack.png",
	"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\nightFront.png"
};

std::vector<const char*> NightTextureFiles = {
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
	this->textureIDNight = loader->LoadCubeMapTexture(NightTextureFiles);

	this->shader = new SkyboxShader();
	this->shader->Start();
	// 告诉着色器需要采样的纹理单元
	this->shader->ConnectTextureUnits();
	this->shader->LoadProjectionMatrix(projection);
	this->shader->Stop();
}

SkyboxRender::~SkyboxRender()
{

}

void SkyboxRender::RenderModel(Camera* camera, glm::vec3 fogColor)
{
	shader->Start();
	shader->LoadViewMatrix(camera);
	shader->LoadFogColor(fogColor);

	glBindVertexArray(cube->GetVAOID());
	glEnableVertexAttribArray(0);

	BindTextures();
	glDrawArrays(GL_TRIANGLES, 0, cube->GetVertexCount());

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	shader->Stop();
}

void SkyboxRender::BindTextures()
{
	// 白天和夜晚交替效果
	double currentSeconds = glfwGetTime();          // 秒
	long long timeMs = (long long)(currentSeconds * 1000) % 24000;

	int texture1;
	int texture2;
	float blendFactor;

	if (timeMs >= 0 && timeMs < 5000) {
		texture1 = textureIDNight;
		texture2 = textureIDNight;
		blendFactor = (float)(timeMs - 0) / (5000 - 0);
	}
	else if (timeMs >= 5000 && timeMs < 8000) {
		texture1 = textureIDNight;
		texture2 = textureID;
		blendFactor = (float)(timeMs - 5000) / (8000 - 5000);
	}
	else if (timeMs >= 8000 && timeMs < 21000) {
		texture1 = textureID;
		texture2 = textureID;
		blendFactor = (float)(timeMs - 8000) / (21000 - 8000);
	}
	else { // 21000 ~ 23999
		texture1 = textureID;
		texture2 = textureIDNight;
		blendFactor = (float)(timeMs - 21000) / (24000 - 21000);
	}
	shader->LoadBlendFactor(blendFactor);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);
}