#include "StdAfx.h"
#include "Maths.h"
#include "Camera.h"

glm::mat4 Maths::CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, float scale)
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
	// 将度数转换为弧度
	float radX = glm::radians(rotation.x);
	float radY = glm::radians(rotation.y);
	float radZ = glm::radians(rotation.z);

	glm::mat4 rotationXMatrix = glm::rotate(glm::mat4(1.0f), radX, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotationYMatrix = glm::rotate(glm::mat4(1.0f), radY, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationZMatrix = glm::rotate(glm::mat4(1.0f), radZ, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale));
	return translationMatrix * rotationZMatrix * rotationYMatrix * rotationXMatrix * scaleMatrix;
}

glm::mat4 Maths::CreateProjectionMatrix(float fvo, float aspectRatio, float nearPlane, float farPlane)
{
	return glm::perspective(glm::radians(fvo), aspectRatio, nearPlane, farPlane);
}

glm::mat4 Maths::CreateViewMatrix(Camera* camera)
{
	glm::mat4 viewMatrix = glm::mat4(1.0f);  // 初始化为单位矩阵
	float pitchRadians = glm::radians(camera->GetPitch());
	float yawRadians = glm::radians(camera->GetYaw());
	viewMatrix = glm::rotate(viewMatrix, pitchRadians, glm::vec3(1, 0, 0));  // 绕x轴旋转
	viewMatrix = glm::rotate(viewMatrix, yawRadians, glm::vec3(0, 1, 0));  // 绕y轴旋转
	glm::vec3 cameraPos = camera->GetPosition();
	glm::vec3 negativeCameraPos = -cameraPos;
	viewMatrix = glm::translate(viewMatrix, negativeCameraPos);  // 平移
	//viewMatrix = glm::inverse(viewMatrix);  // 反转矩阵，因为我们希望摄像机看向的方向是正值方向
	return viewMatrix;
}

float Maths::RandFloat() {
	return (float)rand() / RAND_MAX;
}

float Maths::BarryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
	const glm::vec2& pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}