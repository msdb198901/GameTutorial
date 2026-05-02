#pragma once

class GLFWwindow;
class Camera 
{
public:
	Camera();
	~Camera();

	// 摄像机移动函数
	void Move(GLFWwindow* window, float deltaTime);

	glm::vec3 GetPosition() const;
	float GetYaw() const;
	float GetPitch() const;
	float GetRoll() const;

private:
	glm::vec3 m_position;
	float m_pitch;	// 俯仰角 x 摄像机上下瞄准的角度
	float m_yaw;	// 偏航角 y 摄像机向左或向右转动的幅度
	float m_roll;	// 翻滚角 z 摄像机向一侧倾斜的角度 180 度时摄像机倒立
};