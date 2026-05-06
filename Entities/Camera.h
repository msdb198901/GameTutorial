#pragma once

class GLFWwindow;
class Player;
class Camera 
{
public:
	Camera(Player* player);
	~Camera();

	// 摄像机移动函数
	void Move(GLFWwindow* window, float deltaTime);
	void CalculateZoom(float yoffset);
	void CalculateAngleAroundPlayer(float offsetAngleX);
	void CalculatePitch(float yoffset);

	float CalculateHorizontalDistance() const;
	float CalculateVerticalDistance() const;

	void CalculateCameraPosition();
	void IncreasePosition(float dx, float dy, float dz);

	// 摄像机翻转函数
	void InvertPitch();

	glm::vec3 GetPosition() const;
	float GetYaw() const;
	float GetPitch() const;
	float GetRoll() const;

private:
	glm::vec3 m_position;
	float m_pitch;	// 俯仰角 x 摄像机上下瞄准的角度 摄像机的高低位置
	float m_yaw;	// 偏航角 y 摄像机向左或向右转动的幅度
	float m_roll;	// 翻滚角 z 摄像机向一侧倾斜的角度 180 度时摄像机倒立
	
	float m_distanceFromPlayer;	// 摄像机距离玩家多远
	float m_angleAroundPlayer;	// 摄像机围绕玩家旋转的角度
	Player* m_player;

	public:
	// 用于鼠标移动跟踪
	double lastMouseX = 0.0, lastMouseY = 0.0;
	bool firstMouse = true;

	// 按键状态（由鼠标按钮回调维护）
	bool leftButtonPressed = false;   // 鼠标左键
	bool rightButtonPressed = false;  // 鼠标右键
};