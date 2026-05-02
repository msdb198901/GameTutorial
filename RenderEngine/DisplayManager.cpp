#include "StdAfx.h"
#include "DisplayManager.h"
#include "EntityRender.h"
#include "Loader.h"
#include "RawModel.h"
#include "StaticShader.h"
#include "Texture.h"
#include "TextureModel.h"
#include "Entity.h"
#include "Camera.h"
#include "ObjLoader.h"
#include "Light.h"
#include "MasterRender.h"

#include "Terrain.h"
#include "Maths.h"

float  deltaTime = 0.0f;
float  lastFrame = 0.0f;

void framebuffersizefunc(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void DisplayManager::CreateDisplay()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	lastFrame = glfwGetTime();

	windows = glfwCreateWindow(WIDTH, HEIHGT, "GameTutorial", NULL, NULL);
	if (windows == NULL)
	{
		std::cout << "create windows failed!" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(windows);
	glfwSetFramebufferSizeCallback(windows, framebuffersizefunc);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "load opengl address failed!" << std::endl;
		glfwTerminate();
		return;
	}
}



void DisplayManager::UpdateDisplay()
{
	// 创建加载器对象
	Loader* loader = new Loader();

	// 创建主渲染器
	MasterRender* shader = new MasterRender();

	// 加载模型文件
	RawModel *treeModel = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\tree.obj", loader);
	Texture* treeTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\tree.png"));
	TextureModel* treeTextureModel = new TextureModel(treeModel, treeTexture);

	RawModel *grassModel = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\grassModel.obj", loader);
	Texture* grassTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\grassTexture.png"));
	grassTexture->SetUseFakeLighting(true);
	grassTexture->SetTransparency(true);
	TextureModel* grassTextureModel = new TextureModel(grassModel, grassTexture);

	RawModel *fernModel = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\fern.obj", loader);
	Texture* fernTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\fern.png"));
	fernTexture->SetTransparency(true);
	//fernTexture->SetUseFakeLighting(true);
	TextureModel*fernTextureModel = new TextureModel(fernModel, fernTexture);

	std::vector<Entity*> entities;
	for (int i = 0; i < 500; ++i)
	{
		Entity* tree = new Entity(treeTextureModel, glm::vec3(Maths::RandFloat() * 800 - 400, 0, Maths::RandFloat() * -600), glm::vec3(0, 0, 0), 3);
		entities.push_back(tree);

		Entity* grass = new Entity(grassTextureModel, glm::vec3(Maths::RandFloat() * 800 - 400, 0, Maths::RandFloat() * -600), glm::vec3(0, 0, 0), 1);
		entities.push_back(grass);

		Entity* fern = new Entity(fernTextureModel, glm::vec3(Maths::RandFloat() * 800 - 400, 0, Maths::RandFloat() * -600), glm::vec3(0, 0, 0), 0.6);
		entities.push_back(fern);
	}

	RawModel* stallModel = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\stall.obj", loader);
	Texture* stallTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\stallTexture.png"));
	TextureModel* stallTextureModel = new TextureModel(stallModel, stallTexture);
	Entity* stall = new Entity(stallTextureModel, glm::vec3(0, 0, -25), glm::vec3(0, 0, 0), 1.0);
	entities.push_back(stall);

	// 地形
	Texture* terrainTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\grass.png"));
	Terrain* terrain1 = new Terrain(0, 0, loader, terrainTexture);
	Terrain* terrain2 = new Terrain(1, 0, loader, terrainTexture);
	
	Camera *camera = new Camera();
	Light* light = new Light(glm::vec3(3000, 2000, 2000), glm::vec3(1.0f, 1.0f, 1.0f));

	
	while (!glfwWindowShouldClose(windows))
	{
		processInput();

		// 这个时候变换z轴，物体没有任何变换
		// z轴越远，物体应该越小 所以需要投影矩阵
		// 2、	投影矩阵 视野角度 宽高比 近裁剪面 远裁剪面
		//		视野角度 决定了视野的宽广程度
		//		近裁剪面-远裁剪面 决定了可视距离有多远
		// 3、	视图矩阵 相机位置 向量
		//		场景物体相对于相机向相反方向移动
		//		移动所有场景物体
		stall->IncreaseRotation(0, 0.5f * deltaTime, 0);

		camera->Move(windows, deltaTime);

		shader->ProcessTerrain(terrain1);
		shader->ProcessTerrain(terrain2);
		for (auto entity : entities)
		{
			shader->ProcessEntity(entity);
		}
		shader->RenderModel(light, camera);

		glfwSwapBuffers(windows);
		glfwPollEvents();
	}
	shader->CleanUp();
	// 删除顶点缓冲资源 删除所有顶点对象
	loader->ClearUp();
}

void DisplayManager::CloseDisplay()
{
	glfwTerminate();
}

void DisplayManager::processInput()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (glfwGetKey(windows, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(windows, true);
	}
}