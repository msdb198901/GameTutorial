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

#include "EmissiveEntity.h"
#include "EmissiveShader.h"

#include "MousePicker.h"

#include "WaterShader.h"
#include "WaterTile.h"
#include "WaterRenderer.h"
#include "WaterFrameBuffers.h"

#include "AnimatedEntity.h"
#include <filesystem>
#include <learnopengl/model.h>
#include <learnopengl/animation.h>


float  deltaTime = 0.0f;
float  lastFrame = 0.0f;

int DisplayManager::WIDTH = 1280;
int DisplayManager::HEIHGT = 720;

void framebuffersizefunc(GLFWwindow* window, int width, int height)
{
	DisplayManager::WIDTH = width;
	DisplayManager::HEIHGT = height;
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
	glfwSetFramebufferSizeCallback(windows, [](GLFWwindow* win, int width, int height){
		//MousePicker* picker = static_cast<MousePicker*>(glfwGetWindowUserPointer(win));
		//if (picker) {
		//	picker->SetWidthHeight(width, height);
		//}
		framebuffersizefunc(win, width, height);
	});

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
	MasterRender* render = new MasterRender(loader);

	// 地形
	TerrainTexture* backgroudTexture = new TerrainTexture(loader->LoadTexture("Resources\\grassy.png"));
	TerrainTexture* rTexture = new TerrainTexture(loader->LoadTexture("Resources\\dirt.png"));
	TerrainTexture* gTexture = new TerrainTexture(loader->LoadTexture("Resources\\pinkFlowers.png"));
	TerrainTexture* bTexture = new TerrainTexture(loader->LoadTexture("Resources\\path.png"));
	TerrainTexturePack* terrainTexturePack = new TerrainTexturePack(backgroudTexture, rTexture, gTexture, bTexture);
	TerrainTexture* blendMap = new TerrainTexture(loader->LoadTexture("Resources\\blendMap.png"));

	loader->LoadTexture("Resources\\heightmap.png");
	Terrain* terrain1 = new Terrain(0, 0, loader, terrainTexturePack, blendMap,
		"Resources\\heightmap.png");
	//Terrain* terrain2 = new Terrain(-1, 0, loader, terrainTexturePack, blendMap, "Resources\\heightmap.png");
	std::vector<Terrain*> terrains;
	terrains.push_back(terrain1);

	// 加载模型文件
	RawModel * pineModel = ObjLoader::LoadObjModel("Resources\\pine.obj", loader);
	Texture* pineTexture = new Texture(loader->LoadTexture("Resources\\pine.png"));
	TextureModel* pineTextureModel = new TextureModel(pineModel, pineTexture);

	RawModel *grassModel = ObjLoader::LoadObjModel("Resources\\grassModel.obj", loader);
	Texture* grassTexture = new Texture(loader->LoadTexture("Resources\\grassTexture.png"));
	TextureModel* grassTextureModel = new TextureModel(grassModel, grassTexture);

	//RawModel* flowerModel = ObjLoader::LoadObjModel("Resources\\grassModel.obj", loader);
	Texture* flowerTexture = new Texture(loader->LoadTexture("Resources\\flower.png"));
	TextureModel* flowerTextureModel = new TextureModel(grassModel, flowerTexture);

	RawModel *fernModel = ObjLoader::LoadObjModel("Resources\\fern.obj", loader);
	Texture* fernTexture = new Texture(loader->LoadTexture("Resources\\fern.png"));
	fernTexture->SetNumberOfRows(2);
	TextureModel*fernTextureModel = new TextureModel(fernModel, fernTexture);

	RawModel* bobbleModel = ObjLoader::LoadObjModel("Resources\\lowPolyTree.obj", loader);
	Texture* bobbleTexture = new Texture(loader->LoadTexture("Resources\\lowPolyTree.png"));
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
			float x = Maths::RandFloat() * 800;
			float z = Maths::RandFloat() * 600;
			float y = terrain1->GetHeightOfTerrain(x, z);
			Entity* pine = new Entity(pineTextureModel, glm::vec3(x, y, z), glm::vec3(0, 0, 0), Maths::RandFloat() * 1 + 1);
			entities.push_back(pine);
		}
	}

	RawModel* stallModel = ObjLoader::LoadObjModel("Resources\\stall.obj", loader);
	Texture* stallTexture = new Texture(loader->LoadTexture("Resources\\stallTexture.png"));
	TextureModel* stallTextureModel = new TextureModel(stallModel, stallTexture);
	Entity* stall = new Entity(stallTextureModel, glm::vec3(0, 0, -25), glm::vec3(0, 0, 0), 1.0);
	stall->IncreaseRotation(0, 180.0f, 0);
	entities.push_back(stall);
	
	// 创建玩家
	RawModel* playerModel = ObjLoader::LoadObjModel("Resources\\person.obj", loader);
	Texture* playerTexture = new Texture(loader->LoadTexture("Resources\\playerTexture.png"));
	TextureModel* playerTextureModel = new TextureModel(playerModel, playerTexture);
	Player* player = new Player(playerTextureModel, glm::vec3(153, 5, 120), glm::vec3(0, 100, 0), 1.0);
	entities.push_back(player);

	Camera *camera = new Camera(player);
	Light* light1 = new Light(glm::vec3(0, 10000, -7000), glm::vec3(0.8f, 0.8f, 0.8f));
	Light* light2 = new Light(glm::vec3(185, 10, 293), glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(1, 0.01, 0.002));
	Light* light3 = new Light(glm::vec3(370, 5, 300), glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(1, 0.01, 0.002));
	Light* light4 = new Light(glm::vec3(293, 7, 305), glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(1, 0.01, 0.002));
	std::vector<Light*> lights;
	lights.push_back(light1);
	lights.push_back(light2);
	lights.push_back(light3);
	lights.push_back(light4);
	Light* sunLight = new Light(glm::vec3(0, 10000, -7000), glm::vec3(0.8f, 0.8f, 0.8f));

	// 创建发光物
	RawModel* lampModel = ObjLoader::LoadObjModel("Resources\\lamp.obj", loader);
	Texture* lampTexture = new Texture(loader->LoadTexture("Resources\\lamp.png"));
	TextureModel* lampTextureModel = new TextureModel(lampModel, lampTexture);
	EmissiveEntity* lamp1 = new EmissiveEntity(lampTextureModel, glm::vec3(185, -4.7, 293), glm::vec3(0, 0, 0), 1.0);
	EmissiveEntity* lamp2 = new EmissiveEntity(lampTextureModel, glm::vec3(370, -9.2, 300), glm::vec3(0, 0, 0), 1.0);
	EmissiveEntity* lamp3 = new EmissiveEntity(lampTextureModel, glm::vec3(293, -6.8, 305), glm::vec3(0, 0, 0), 1.0);
	lamp1->SetEmissiveColor(glm::vec3(2.0f, 0.0f, 0.0f));
	lamp2->SetEmissiveColor(glm::vec3(0.0f, 2.0f, 2.0f));
	lamp3->SetEmissiveColor(glm::vec3(2.0f, 2.0f, 0.0f));
	std::vector<EmissiveEntity*> emissiveEntities;
	emissiveEntities.push_back(lamp1);
	emissiveEntities.push_back(lamp2);
	emissiveEntities.push_back(lamp3);

	// 创建水面
	WaterShader* waterShader = new WaterShader();
	WaterFrameBuffers* waterFrameBuffers = new WaterFrameBuffers();
	WaterRenderer* waterRender = new WaterRenderer(loader, waterShader, render->GetProjectionMatrix(), waterFrameBuffers);
	std::vector<WaterTile*> waterTiles;
	WaterTile *water = new WaterTile(120, 120, -5);
	waterTiles.push_back(water);

	// 创建 GUI
	std::list<GuiTexture*> guis;
	GuiRender* guiRender = new GuiRender(loader);

	// 创建鼠标拾取器
	//MousePicker* picker = new MousePicker(camera, render->GetProjectionMatrix(), terrain1, WIDTH, HEIHGT);

	// 加载模型和动画
	std::filesystem::path path = "E:\\Learn\\OpenGL\\GameTutorial\\Resources\\objects\\vampire\\dancing_vampire .dae";
	Model* myModel = new Model(path.generic_u8string());
	Animation* idleAnim = new Animation(path.generic_string(), myModel);
	//Animation* walkAnim = new Animation("resources/character/walk.fbx", myModel);
	// 
	// 创建骨骼实体
	AnimatedEntity* character = new AnimatedEntity(myModel, idleAnim);
	character->SetPosition(glm::vec3(100, 5, 100));
	character->SetScale(5.0f);

	// 加载模型和动画
	std::filesystem::path path1 = "E:\\Learn\\OpenGL\\GameTutorial\\Resources\\objects\\nanosuit\\nanosuit.obj";
	Model* nanosuit = new Model(path1.generic_string());
	AnimatedEntity* nanosuitEntity = new AnimatedEntity(nanosuit, nullptr);
	nanosuitEntity->SetPosition(glm::vec3(150, 5, 150));
	nanosuitEntity->SetScale(0.5f);

	std::vector<AnimatedEntity*> animatedEntities;
	animatedEntities.push_back(character);
	animatedEntities.push_back(nanosuitEntity);

	// 设置鼠标滚动回调函数
	glfwSetWindowUserPointer(windows, camera);
	//glfwSetWindowUserPointer(windows, picker);
	
	while (!glfwWindowShouldClose(windows))
	{
		processInput();

		camera->Move(windows, deltaTime);
		player->Move(windows, deltaTime, terrain1);
		character->Update(deltaTime);

		static glm::vec3 lastPos = character->GetPosition();
		if (lastPos != character->GetPosition())
		{
			std::cout << "Position changed to: " << character->GetPosition().x << ", " << character->GetPosition().y << ", " << character->GetPosition().z << std::endl;
			lastPos = character->GetPosition();
		}

		//// 相机移动后再更新鼠标拾取器
		//picker->Update();
		//glm::vec3 terrainPoint = picker->GetCurrentTerrainPoint();
		//if (terrainPoint != glm::vec3(0, 0, 0))
		//{
		//	lamp1->SetPosition(terrainPoint);
		//	light2->SetPosition(terrainPoint);
		//}
		glEnable(GL_CLIP_DISTANCE0);

		waterFrameBuffers->BindReflectionFrameBuffer();
		// 渲染水面反射场景 (水面之上) 原点的距离就等于水面高度
		float distance = 2 * (camera->GetPosition().y - water->GetHeight());
		camera->IncreasePosition(0, -distance, 0);
		camera->InvertPitch();
		render->RenderScene(entities, animatedEntities, emissiveEntities, terrains, lights, camera, glm::vec4(0.0f, 1.0f, 0.0f, -water->GetHeight()+1));
		camera->IncreasePosition(0, distance, 0);
		camera->InvertPitch();

		// 渲染水面折射场景 (水面之下) 原点的距离就等于-水面高度
		waterFrameBuffers->BindRefractionFrameBuffer();
		render->RenderScene(entities, animatedEntities, emissiveEntities, terrains, lights, camera, glm::vec4(0, -1.0f, 0.0f, water->GetHeight()+1));
		waterFrameBuffers->UnBindCurrentFrameBuffer(WIDTH, HEIHGT);
		
		glDisable(GL_CLIP_DISTANCE0);

		render->RenderScene(entities, animatedEntities, emissiveEntities, terrains, lights, camera, glm::vec4(0, 1, 0, 1000));
		//if (isWalking) character->PlayAnimation(walkAnim);
		
		waterRender->RenderModel(waterTiles, camera, sunLight);
		guiRender->RenderModel(guis);

		glfwSwapBuffers(windows);
		glfwPollEvents();
	}
	waterFrameBuffers->CleanUp();
	waterRender->CleanUp();
	guiRender->CleanUp();
	render->CleanUp();
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