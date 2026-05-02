#include "StdAfx.h"
#include "DisplayManager.h"
#include "Render.h"
#include "Loader.h"
#include "RawModel.h"
#include "StaticShader.h"
#include "Texture.h"
#include "TextureModel.h"
#include "Entity.h"
#include "Camera.h"
#include "ObjLoader.h"
#include "Light.h"

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
	// 创建静态渲染着色器
	StaticShader *shader = new StaticShader();
	// 创建渲染器对象
	Render* render = new Render(shader);
	// 加载模型文件
	RawModel *model = ObjLoader::LoadObjModel("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\dragon.obj", loader);

	Texture* texture = new Texture(loader->LoadTexture("E:\\Learn\\OpenGL\\GameTutorial\\Resources\\white.png"));
	texture->SetShineDamper(10);
	texture->SetReflectivity(1.0);

	TextureModel* textureModel = new TextureModel(model, texture);
	Entity* entity = new Entity(textureModel, glm::vec3(0, 0, -25), glm::vec3(0,0,0), 1.0);
	Camera *camera = new Camera();
	Light* light = new Light(glm::vec3(0, 0, -10), glm::vec3(1.0f, 1.0f, 1.0f));

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
		entity->IncreaseRotation(0.0f, 0.001f, 0.0f);

		camera->Move(windows, deltaTime);

		render->Prepare();

		// 渲染模型之前启动着色器
		shader->Start();

		shader->LoadLight(light);

		shader->LoadViewMatrix(camera);

		render->RenderModel(entity, shader);

		shader->Stop();

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