#pragma once

class Camera;
class Terrain;
class MousePicker {
public:
    MousePicker(Camera *camera, const glm::mat4& projection, Terrain* terrain, float screenWidth, float screenHeight);
    void Update();          // 每帧更新，传入当前视图矩阵
    const glm::vec3& GetCurrentRay() const { return currentRay; }
    const glm::vec3& GetCurrentTerrainPoint() const { return currentTerrainPoint; }
    void SetWidthHeight(int width, int height);

private:
    void calculateMouseRay();
    glm::vec2 getNormalizedDeviceCoords(float mouseX, float mouseY) const;
    glm::vec4 toEyeCoords(const glm::vec4& clipCoords) const;
    glm::vec3 toWorldCoords(const glm::vec4& eyeCoords, const glm::mat4& view) const;

    glm::vec3 getPointOnRay(const glm::vec3& ray, float distance) const;
    glm::vec3 binarySearch(int count, float start, float finish, const glm::vec3& ray);

    bool intersectionInRange(float start, float finish, const glm::vec3& ray) const;
    bool isUnderGround(const glm::vec3& testPoint) const;
    Terrain* getTerrain(float worldX, float worldZ) const;

    Camera*     camera;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 currentRay;
    float screenWidth, screenHeight;

    Terrain *terrain;
    glm::vec3 currentTerrainPoint;
};