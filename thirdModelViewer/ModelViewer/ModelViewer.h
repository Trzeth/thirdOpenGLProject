#pragma once

#include <thirdEngine/Renderer/Renderer.h>
#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Input/Input.h>
#include <thirdEngine/Renderer/UI/UIRenderer.h>

#include "System/ModelRenderSystem.h"
#include "System/CameraSystem.h"
#include "System/ViewerInputSystem.h"
#include "System/InspectorSystem.h"

#include "Window.h"

#include "Scene/TestScene.h"

constexpr double FRAMEDURATION = 1.0 / 60;

class ModelViewer
{
public:
	ModelViewer();
	int Run();

private:
	int setup();
	int loop();
	int teardown();

	void update();
	void draw();

	bool wireframe;
	bool running;
	bool restart;
	bool started;

	std::unique_ptr<ModelRenderSystem> modelRenderSystem;
	std::unique_ptr<CameraSystem> cameraSystem;
	std::unique_ptr<ViewerInputSystem> viewInputSystem;
	std::unique_ptr<InspectorSystem> inspectorSystem;

	std::unique_ptr<TestScene> defaultScene;

	Renderer renderer;
	UIRenderer uiRenderer;
	World world;
	Input input;
	Window window;
	EventManager eventManager;

	//两帧之间的时间差
	double currentFrame;
	double lastFrame;
	double deltaTime;
};
