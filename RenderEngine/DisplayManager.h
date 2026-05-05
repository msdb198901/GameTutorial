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
	static int WIDTH ;
	static int HEIHGT;

private:
	GLFWwindow* windows;
};