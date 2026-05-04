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

#include "TerrainTexture.h"
#include "TerrainTexturePack.h"
#include "Player.h"

#include "GuiTexture.h"
#include "GuiRender.h"

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
	// 1. 滚轮回调 —— 缩放
	glfwSetScrollCallback(windows, [](GLFWwindow* win, double xoffset, double yoffset) {
		Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(win));
		if (camera) {
			camera->CalculateZoom((float)yoffset);
		}
		return;
		});

	// 2. 鼠标按钮回调 —— 记录按键状态
	glfwSetMouseButtonCallback(windows, [](GLFWwindow* win, int button, int action, int mods) {
		Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(win));
		if (!camera) return;

		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			camera->leftButtonPressed = (action == GLFW_PRESS);
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			camera->rightButtonPressed = (action == GLFW_PRESS);
		}
	});

	// 3. 鼠标移动回调 —— 根据按键状态更新 pitch 或 angleAroundPlayer
	glfwSetCursorPosCallback(windows, [](GLFWwindow* win, double xpos, double ypos) {
		Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(win));
		if (!camera) return;

		// 初始化上一次鼠标位置
		if (camera->firstMouse) {
			camera->lastMouseX = xpos;
			camera->lastMouseY = ypos;
			camera->firstMouse = false;
			return;
		}

		// 计算本次移动差值
		float dx = static_cast<float>(xpos - camera->lastMouseX);
		float dy = static_cast<float>(ypos - camera->lastMouseY);

		// 根据按下->的按钮调用相应方法
		if (camera->leftButtonPressed) {
			camera->CalculateAngleAroundPlayer(dx);
		}
		if (camera->rightButtonPressed) {
			camera->CalculatePitch(dy);
		}

		// 更新上一次鼠标位置
		camera->lastMouseX = xpos;
		camera->lastMouseY = ypos;
		});

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

	// 地形
	TerrainTexture* backgroudTexture = new TerrainTexture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\grassy.png"));
	TerrainTexture* rTexture = new TerrainTexture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\dirt.png"));
	TerrainTexture* gTexture = new TerrainTexture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\pinkFlowers.png"));
	TerrainTexture* bTexture = new TerrainTexture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\path.png"));
	TerrainTexturePack* terrainTexturePack = new TerrainTexturePack(backgroudTexture, rTexture, gTexture, bTexture);
	TerrainTexture* blendMap = new TerrainTexture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\blendMap.png"));

	loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\heightmap.png");
	Terrain* terrain1 = new Terrain(0, 0, loader, terrainTexturePack, blendMap,
		"E:\\Learn\\OpenGL\\GameTutorial\\Resources\\heightmap.png");
	//Terrain* terrain2 = new Terrain(-1, 0, loader, terrainTexturePack, blendMap, "E:\\Learn\\OpenGL\\GameTutorial\\Resources\\heightmap.png");


	// 加载模型文件
	RawModel * pineModel = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\pine.obj", loader);
	Texture* pineTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\pine.png"));
	TextureModel* pineTextureModel = new TextureModel(pineModel, pineTexture);

	RawModel *grassModel = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\grassModel.obj", loader);
	Texture* grassTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\grassTexture.png"));
	TextureModel* grassTextureModel = new TextureModel(grassModel, grassTexture);

	//RawModel* flowerModel = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\grassModel.obj", loader);
	Texture* flowerTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\flower.png"));
	TextureModel* flowerTextureModel = new TextureModel(grassModel, flowerTexture);

	RawModel *fernModel = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\fern.obj", loader);
	Texture* fernTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\fern.png"));
	fernTexture->SetNumberOfRows(2);
	TextureModel*fernTextureModel = new TextureModel(fernModel, fernTexture);

	RawModel* bobbleModel = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\lowPolyTree.obj", loader);
	Texture* bobbleTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\lowPolyTree.png"));
	TextureModel* bobbleTextureModel = new TextureModel(bobbleModel, bobbleTexture);

	grassTexture->SetUseFakeLighting(true);
	grassTexture->SetTransparency(true);
	flowerTexture->SetUseFakeLighting(true);
	flowerTexture->SetTransparency(true);
	fernTexture->SetTransparency(true);

	std::vector<Entity*> entities;
	for (int i = 0; i < 400; ++i)
	{
		if (i % 2 == 0)
		{
			float x = Maths::RandFloat() * 400;
			float z = Maths::RandFloat() * 400;
			float y = terrain1->GetHeightOfTerrain(x, z);
			Entity* fern = new Entity(fernTextureModel, rand() % 4, glm::vec3(x, y, z), glm::vec3(0, Maths::RandFloat() * 360, 0), 0.9);
			entities.push_back(fern);
		}

		if (i % 3 == 0)
		{
			//float x = Maths::RandFloat() * 800;
			//float z = Maths::RandFloat() * 600;
			//float y = terrain1->GetHeightOfTerrain(x, z);
			//Entity* bobble = new Entity(bobbleTextureModel, glm::vec3(x, y, z), glm::vec3(0, Maths::RandFloat() * 360, 0), Maths::RandFloat() * 0.1 + 0.6);
			//entities.push_back(bobble);	

			float x = Maths::RandFloat() * 800;
			float z = Maths::RandFloat() * 600;
			float y = terrain1->GetHeightOfTerrain(x, z);
			Entity* pine = new Entity(pineTextureModel, glm::vec3(x, y, z), glm::vec3(0, 0, 0), Maths::RandFloat() * 1 + 1);
			entities.push_back(pine);
		}
	}

	RawModel* stallModel = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\stall.obj", loader);
	Texture* stallTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\stallTexture.png"));
	TextureModel* stallTextureModel = new TextureModel(stallModel, stallTexture);
	Entity* stall = new Entity(stallTextureModel, glm::vec3(0, 0, -25), glm::vec3(0, 0, 0), 1.0);
	stall->IncreaseRotation(0, 180.0f, 0);
	entities.push_back(stall);
	
	// 创建玩家
	RawModel* playerModel = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\person.obj", loader);
	Texture* playerTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\playerTexture.png"));
	TextureModel* playerTextureModel = new TextureModel(playerModel, playerTexture);
	Player* player = new Player(playerTextureModel, glm::vec3(153, 5, 273), glm::vec3(0, 100, 0), 1.0);

	Camera *camera = new Camera(player);
	Light* light1 = new Light(glm::vec3(0, 10000, -7000), glm::vec3(0.4f, 0.4f, 0.4f));
	Light* light2 = new Light(glm::vec3(185, 10, 293), glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(1, 0.01, 0.002));
	Light* light3 = new Light(glm::vec3(370, 17, 300), glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(1, 0.01, 0.002));
	Light* light4 = new Light(glm::vec3(293, 7, 305), glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(1, 0.01, 0.002));
	std::vector<Light*> lights;
	lights.push_back(light1);
	lights.push_back(light2);
	lights.push_back(light3);
	lights.push_back(light4);

	// 创建发光物
	RawModel* lampModel = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\lamp.obj", loader);
	Texture* lampTexture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\lamp.png"));
	TextureModel* lampTextureModel = new TextureModel(lampModel, lampTexture);
	Entity* lamp1 = new Entity(lampTextureModel, glm::vec3(185, -4.7, 293), glm::vec3(0, 0, 0), 1.0);
	Entity* lamp2 = new Entity(lampTextureModel, glm::vec3(370, 4.2, 300), glm::vec3(0, 0, 0), 1.0);
	Entity* lamp3 = new Entity(lampTextureModel, glm::vec3(293, -6.8, 305), glm::vec3(0, 0, 0), 1.0);
	entities.push_back(lamp1);
	entities.push_back(lamp2);
	entities.push_back(lamp3);

	// 设置鼠标滚动回调函数
	glfwSetWindowUserPointer(windows, camera);

	// 创建 GUI
	std::list<GuiTexture*> guis;
	GuiTexture* gui = new GuiTexture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\socuwan.png"), 
		glm::vec2(0.5), glm::vec2(0.25));
	GuiTexture* gui2 = new GuiTexture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\thinmatrix.png"),
		glm::vec2(0.3, 0.74), glm::vec2(0.4));
	guis.push_back(gui);
	guis.push_back(gui2);

	GuiRender* guiRenderer = new GuiRender(loader);

	while (!glfwWindowShouldClose(windows))
	{
		processInput();

		camera->Move(windows, deltaTime);
		player->Move(windows, deltaTime, terrain1);
		shader->ProcessEntity(player);

		shader->ProcessTerrain(terrain1);
		//shader->ProcessTerrain(terrain2);
		for (auto entity : entities)
		{
			shader->ProcessEntity(entity);
		}
		shader->RenderModel(lights, camera);

		guiRenderer->RenderModel(guis);

		glfwSwapBuffers(windows);
		glfwPollEvents();
	}
	guiRenderer->CleanUp();
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