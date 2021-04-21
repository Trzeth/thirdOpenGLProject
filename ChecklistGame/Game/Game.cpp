#include "Game.h"

#include "Scene/Home/YardScene.h"

Game::Game() :renderer(), uiRenderer(), world(), input(), window(input, renderer), eventManager(world)
{
	restart = false;
	running = false;
	wireframe = false;
	started = false;

	lastFrame = 0;
	currentFrame = 0;
	deltaTime = 0;
}

int Game::Run()
{
	if (setup() < 0) {
		return -1;
	}

	running = true;
	loop();

	teardown();

	return 0;
}

int Game::setup()
{
	window.Initialize();

	/* Input */
	input.Initialize();
	input.SetDefaultMapping("Horizontal", KbmAxis_D, KbmAxis_A);
	input.SetDefaultMapping("Vertical", KbmAxis_W, KbmAxis_S);
	input.SetDefaultMapping("LookHorizontal", KbmAxis_MouseXPos, KbmAxis_MouseXNeg, AxisProps(0.1f, 0.2f, 0.3f));
	input.SetDefaultMapping("LookVertical", KbmAxis_MouseYPos, KbmAxis_MouseYNeg, AxisProps(0.1f, 0.2f, 0.3f));

	input.SetDefaultMapping("Space", KbmAxis_Space, KbmAxis_None);
	input.SetDefaultMapping("E", KbmAxis_E, KbmAxis_None);
	input.SetDefaultMapping("LeftMouse", KbmAxis_MouseLeft, KbmAxis_None);
	input.SetDefaultMapping("R", KbmAxis_R, KbmAxis_None);
	input.SetDefaultMapping("Return", KbmAxis_Return, KbmAxis_None);

	/* Event Manager */

	/* Renderer*/
	renderer.Initialize(window.GetWidth(), window.GetHeight());
	uiRenderer.Initialize(window.GetWindow());

	/* System */
	modelRenderSystem = std::make_unique<ModelRenderSystem>(world, renderer);
	cameraSystem = std::make_unique<CameraSystem>(world, renderer);
	playerInputSystem = std::make_unique<PlayerInputSystem>(world, input, eventManager);
	playerAnimationStateSystem = std::make_unique<PlayerAnimationStateSystem>(world, renderer);
	playerControlStateSystem = std::make_unique<PlayerControlStateSystem>(world, window);
	storyboardDirectorSystem = std::make_unique<StoryboardDirectorSystem>(world, eventManager);

	SceneInfo sceneInfo;
	sceneInfo.world = &world;
	sceneInfo.renderer = &renderer;
	sceneInfo.uiRenderer = &uiRenderer;
	defaultScene = std::make_unique<YardScene>(sceneInfo, eventManager);
	defaultScene->Setup();

	return 0;
}

int Game::loop()
{
	while (!window.ShouldClose()) {
		currentFrame = window.GetTime();
		if (!lastFrame)lastFrame = currentFrame;
		deltaTime = currentFrame - lastFrame;

		if (deltaTime < FRAMEDURATION)continue;

		update();
		draw();

		lastFrame = currentFrame;
	}
	return 0;
}

int Game::teardown()
{
	window.Close();
	return 0;
}

void Game::update()
{
	/* Input */
	input.Update();
	playerInputSystem->Update(deltaTime);

	/* Display */
	storyboardDirectorSystem->Update(deltaTime);
	playerAnimationStateSystem->Update(deltaTime);
	playerControlStateSystem->Update(deltaTime);

	modelRenderSystem->Update(deltaTime);
	cameraSystem->Update(deltaTime);

	renderer.Update(deltaTime);
	uiRenderer.Update(deltaTime);
	world.CleanupEntities();
}

void Game::draw()
{
	renderer.Draw();
	uiRenderer.Draw();
	window.NextFrame();
}