#include "StdAfx.h"
#include "MousePicker.h"
#include "Maths.h"
#include "Camera.h"
#include "Terrain.h"

const int RECURSION_COUNT = 200;
const float RAY_RANGE = 600;

MousePicker::MousePicker(Camera* camera, const glm::mat4& projection, Terrain* terrain, float screenWidth, float screenHeight)
    : camera(camera), projectionMatrix(projection), terrain(terrain),  screenWidth(screenWidth), screenHeight(screenHeight) {
    Update();
}

void MousePicker::Update() {
    viewMatrix = Maths::CreateViewMatrix(camera);
	calculateMouseRay();
	if (intersectionInRange(0, RAY_RANGE, currentRay)) {
		currentTerrainPoint = binarySearch(0, 0, RAY_RANGE, currentRay);
	}
	else {
		currentTerrainPoint = glm::vec3(0.0f);
	}
}

void MousePicker::SetWidthHeight(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
    projectionMatrix = Maths::CreateProjectionMatrix(70, width / height, 0.1, 1000.0f);
}

void MousePicker::calculateMouseRay()
{
	double mouseX, mouseY;
	glfwGetCursorPos(glfwGetCurrentContext(), &mouseX, &mouseY);   // 获取鼠标坐标
	// 生成射线
	glm::vec2 normalizedCoords = getNormalizedDeviceCoords((float)mouseX, (float)mouseY);
	glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
	glm::vec4 eyeCoords = toEyeCoords(clipCoords);
	currentRay = toWorldCoords(eyeCoords, viewMatrix);
}

glm::vec2 MousePicker::getNormalizedDeviceCoords(float mouseX, float mouseY) const {
    // 将鼠标坐标映射到 [-1, 1] 范围，注意 OpenGL 中 Y 轴向上，需要翻转
    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight;
    return glm::vec2(x, y);
}

glm::vec4 MousePicker::toEyeCoords(const glm::vec4& clipCoords) const {
    // 使用投影矩阵的逆矩阵将裁剪坐标转换到眼睛坐标
    // 注意：这里需要你传入当前使用的投影矩阵，或者作为成员存储
    glm::mat4 invProj = glm::inverse(projectionMatrix);
    glm::vec4 eyeCoords = invProj * clipCoords;
    return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);   // 方向向量，w=0 表示方向而非位置
}

glm::vec3 MousePicker::toWorldCoords(const glm::vec4& eyeCoords, const glm::mat4& view) const {
    glm::mat4 invView = glm::inverse(view);
    glm::vec4 rayWorld = invView * eyeCoords;
    glm::vec3 mouseRay = glm::normalize(glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z));
    return mouseRay;
}

// 修正：返回射线起点 + 方向 * 距离
glm::vec3 MousePicker::getPointOnRay(const glm::vec3& ray, float distance) const {
    glm::vec3 start = camera->GetPosition();
    return start + ray * distance;
}

// 递归查找地面交点
glm::vec3 MousePicker::binarySearch(int count, float start, float finish, const glm::vec3& ray) {
    float half = start + (finish - start) * 0.5f;
    if (count >= RECURSION_COUNT) {
        glm::vec3 endPoint = getPointOnRay(ray, half);
        // 检查是否真的与地形相交（这里简单返回终点）
        if (getTerrain(endPoint.x, endPoint.z) != nullptr) {
            return endPoint;
        }
        else {
            return glm::vec3(0.0f);
        }
    }
    if (intersectionInRange(start, half, ray)) {
        return binarySearch(count + 1, start, half, ray);
    }
    else {
        return binarySearch(count + 1, half, finish, ray);
    }
}

bool MousePicker::intersectionInRange(float start, float finish, const glm::vec3& ray) const {
    glm::vec3 startPoint = getPointOnRay(ray, start);
    glm::vec3 endPoint = getPointOnRay(ray, finish);
    // 如果起点在地面以上，终点在地面以下，则射线与地形相交
    return (!isUnderGround(startPoint) && isUnderGround(endPoint));
}

bool MousePicker::isUnderGround(const glm::vec3& testPoint) const {
    Terrain* t = getTerrain(testPoint.x, testPoint.z);
    if (t == nullptr) return false;
    float terrainHeight = t->GetHeightOfTerrain(testPoint.x, testPoint.z);
    return (testPoint.y < terrainHeight);
}

// 根据世界坐标获取地形（若只有一个地形块，直接返回成员；如果有多块，需要遍历）
Terrain* MousePicker::getTerrain(float worldX, float worldZ) const {
    // 如果整个场景只有一个地形对象，直接返回 terrain。
    // 如果有多个地形块，需要根据 worldX, worldZ 找到对应的地形。
    // 这里假设你储存了一个地形列表（如 std::vector<Terrain*> terrains），需要遍历判断。
    // 下述示例为单个地形的简单返回：
    return terrain;
}