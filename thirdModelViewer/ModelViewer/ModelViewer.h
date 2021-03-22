#pragma once

#include <thirdEngine/Renderer/Renderer.h>
#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Input/Input.h>

#include "System/ModelRenderSystem.h"
#include "System/CameraSystem.h"

#include "Window.h"

#include "Scene/DefaultScene.h"

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
	//std::unique_ptr<PlayerInputSystem> playerInputSystem;
	std::unique_ptr<DefaultScene> defaultScene;

	Renderer renderer;
	World world;
	Input input;
	Window window;
	EventManager eventManager;

	//两帧之间的时间差
	double currentFrame;
	double lastFrame;
	double deltaTime;
};
