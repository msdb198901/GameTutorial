#pragma once

class GLFWwindow;
class Render;
class DisplayManager
{
public:
	void CreateDisplay();
	void UpdateDisplay();
	void CloseDisplay();

private:
	void processInput();

public:
	const static int WIDTH = 1280;
	const static int HEIHGT = 720;

private:
	GLFWwindow* windows;
};