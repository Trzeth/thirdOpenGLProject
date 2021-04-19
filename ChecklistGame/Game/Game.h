#pragma once

#include <thirdEngine/Renderer/Renderer.h>
#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Input/Input.h>
#include <thirdEngine/Renderer/UI/UIRenderer.h>

#include "System/ModelRenderSystem.h"
#include "System/CameraSystem.h"
#include "System/PlayerInputSystem.h"
#include "System/StoryboardDirectorSystem.h"

#include "Window.h"

#include "Scene/Scene.h"

constexpr double FRAMEDURATION = 1.0 / 60;

class Game
{
public:
	Game();
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
	std::unique_ptr<PlayerInputSystem> playerInputSystem;
	std::unique_ptr<StoryboardDirectorSystem> storyboardDirectorSystem;

	std::unique_ptr<Scene> defaultScene;

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
