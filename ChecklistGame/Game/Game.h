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
#include "System/PlayerAnimationStateSystem.h"
#include "System/PlayerControlStateSystem.h"
#include "System/CollisionUpdateSystem.h"
#include "System/RigidbodyMotorSystem.h"

#include "Window.h"
#include "Physics.h"
#include "PhysicsDebugDrawer.h"
#include "GlobalVariable.h"

#include "Scene/Scene.h"

constexpr double FRAMEDURATION = 1.0 / 60;
constexpr int VELOCITYITERATION = 10;
constexpr int POSITIONITERATION = 8;

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
	std::unique_ptr<PlayerAnimationStateSystem> playerAnimationStateSystem;
	std::unique_ptr<PlayerControlStateSystem> playerControlStateSystem;
	std::unique_ptr<StoryboardDirectorSystem> storyboardDirectorSystem;
	std::unique_ptr<CollisionUpdateSystem> collisionUpdateSystem;
	std::unique_ptr<RigidbodyMotorSystem> rigidbodyMotorSystem;

	std::unique_ptr<Scene> defaultScene;
	std::shared_ptr<b2World> dynamicsWorld;

	Renderer renderer;
	Physics physics;
	PhysicsDebugDrawer debugDrawer;
	UIRenderer uiRenderer;
	World world;
	Input input;
	Window window;
	EventManager eventManager;
	GlobalVariable globalVariable;

	//两帧之间的时间差
	double currentFrame;
	double lastFrame;
	double deltaTime;
};
