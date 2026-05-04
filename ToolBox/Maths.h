#pragma once

class Camera;
class Maths
{
public:
	// 平移 旋转 缩放 创建一个变换矩阵
	static glm::mat4 CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, float scale);

	static glm::mat4 CreateProjectionMatrix(float fvo, float aspectRatio, float nearPlane, float farPlane);

	static glm::mat4 CreateViewMatrix(Camera* camera);

	static float RandFloat();

	static float BarryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
							   const glm::vec2& pos);
};