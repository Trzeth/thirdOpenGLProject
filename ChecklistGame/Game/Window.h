#pragma once
#ifndef _Window_h_
#define _Window_h_
#include <GLFW/glfw3.h>

class Input;
class Renderer;

class Window
{
public:
	Window(Input& input, Renderer& renderer);
	void Initialize();

	void MakeContextCurrent();
	void NextFrame();
	void SetCloseFlag(bool flag);
	bool ShouldClose();
	void Close();

	double GetTime();
	GLFWwindow* GetWindow();
	int GetWidth();
	int GetHeight();

private:
	GLFWwindow* window;
	Input& input;
	Renderer& renderer;

	int width = 500, height = 500;
	bool firstMouse = true;
	float lastX, lastY;

	bool windowCloseFlag = false;

	void setupGLFW();
	void create();
	void setupCallBack();

#pragma region Call Back Func
	friend struct ScrollCallback;
	friend struct MouseCallback;
	friend struct FramebufferSizeCallback;
	friend struct KeyCallback;
	friend struct MouseButtonCallBack;
#pragma endregion
};
#endif // !_Window_h_
