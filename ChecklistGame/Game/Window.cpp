#include "Window.h"

#include <cassert>
#include <iostream>

#include <thirdEngine/Input/Input.h>
#include <thirdEngine/Renderer/Renderer.h>

#pragma region Call Back

struct ScrollCallback {
	static Window* obj;

	static void callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		obj->input.HandleScrollCallback(xoffset, yoffset);
	}
};
Window* ScrollCallback::obj;

struct MouseCallback {
	static Window* obj;

	static void callback(GLFWwindow* window, double xpos, double ypos) {
		if (obj->firstMouse)
		{
			obj->lastX = xpos;
			obj->lastY = ypos;
			obj->firstMouse = false;
		}

		float xoffset = xpos - obj->lastX;
		float yoffset = obj->lastY - ypos; // reversed since y-coordinates go from bottom to top

		obj->lastX = xpos;
		obj->lastY = ypos;

		obj->input.HandleMouseCallback(xoffset, yoffset);
	}
};
Window* MouseCallback::obj;

struct MouseButtonCallBack {
	static Window* obj;

	static void callback(GLFWwindow* window, int button, int action, int mods) {
		obj->input.HandleMouseButtonCallBack(button, action, mods);
	}
};
Window* MouseButtonCallBack::obj;

struct FramebufferSizeCallback {
	static Window* obj;

	static void callback(GLFWwindow* window, int w, int h) {
		obj->width = w;
		obj->height = h;

		obj->renderer.SetViewport(w, h);
	}
};
Window* FramebufferSizeCallback::obj;

struct KeyCallback {
	static Window* obj;

	static void callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, true);

		obj->input.HandleKeyCallBack(key, scancode, action, mods);
	}
};
Window* KeyCallback::obj;

#pragma endregion

void Window::setupGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
}

void Window::create()
{
	window = glfwCreateWindow(width, height, "Checklist", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		assert(0);
	}
	glfwGetWindowSize(window, &width, &height);
}

void Window::setupCallBack()
{
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	ScrollCallback::obj = this;
	MouseCallback::obj = this;
	MouseButtonCallBack::obj = this;
	FramebufferSizeCallback::obj = this;
	KeyCallback::obj = this;

	glfwSetScrollCallback(window, ScrollCallback::callback);
	glfwSetCursorPosCallback(window, MouseCallback::callback);
	glfwSetMouseButtonCallback(window, MouseButtonCallBack::callback);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback::callback);
	glfwSetKeyCallback(window, KeyCallback::callback);
}

Window::Window(Input& input, Renderer& renderer)
	:input(input), renderer(renderer), lastX(0), lastY(0), window(nullptr) { }

void Window::Initialize()
{
	setupGLFW();
	create();

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	setupCallBack();
}

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(window);
}

void Window::NextFrame()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::SetCloseFlag(bool flag)
{
	windowCloseFlag = flag;
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window) || windowCloseFlag;
}

void Window::Close()
{
	glfwTerminate();
}

double Window::GetTime()
{
	return glfwGetTime();
}

int Window::GetWidth()
{
	return width;
}

GLFWwindow* Window::GetWindow()
{
	return window;
}

int Window::GetHeight()
{
	return height;
}