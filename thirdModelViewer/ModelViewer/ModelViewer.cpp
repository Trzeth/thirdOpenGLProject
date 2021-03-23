#include "ModelViewer.h"

#include "Scene/DefaultScene.h"

ModelViewer::ModelViewer() :renderer(), world(), input(), window(input, renderer), eventManager(world)
{
	restart = false;
	running = false;
	wireframe = false;
	started = false;

	lastFrame = 0;
	currentFrame = 0;
	deltaTime = 0;
}

int ModelViewer::Run()
{
	if (setup() < 0) {
		return -1;
	}

	running = true;
	loop();

	teardown();

	return 0;
}

int ModelViewer::setup()
{
	window.Initialize();

	/* Input */
	input.Initialize();
	input.SetDefaultMapping("Horizontal", KbmAxis_D, KbmAxis_A);
	input.SetDefaultMapping("Vertical", KbmAxis_W, KbmAxis_S);
	input.SetDefaultMapping("LookHorizontal", KbmAxis_MouseXPos, KbmAxis_MouseXNeg, AxisProps(0.1f, 0.2f, 0.3f));
	input.SetDefaultMapping("LookVertical", KbmAxis_MouseYPos, KbmAxis_MouseYNeg, AxisProps(0.1f, 0.2f, 0.3f));
	input.SetDefaultMapping("Jump", KbmAxis_Space, KbmAxis_None);
	input.SetDefaultMapping("Use", KbmAxis_E, KbmAxis_None);
	input.SetDefaultMapping("Fire", KbmAxis_MouseLeft, KbmAxis_None);
	input.SetDefaultMapping("Reload", KbmAxis_R, KbmAxis_None);
	input.SetDefaultMapping("Start", KbmAxis_Return, KbmAxis_None);

	/* Event Manager */

	/* Renderer*/
	renderer.Initialize();

	/* System */
	modelRenderSystem = std::make_unique<ModelRenderSystem>(world, renderer);
	cameraSystem = std::make_unique<CameraSystem>(world, renderer);
	viewInputSystem = std::make_unique<ViewerInputSystem>(world, input, eventManager);

	SceneInfo sceneInfo;
	sceneInfo.world = &world;
	sceneInfo.renderer = &renderer;
	defaultScene = std::make_unique<DefaultScene>(sceneInfo, eventManager);
	defaultScene->Setup();

	return 0;
}

int ModelViewer::loop()
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

int ModelViewer::teardown()
{
	window.Close();
	return 0;
}

void ModelViewer::update()
{
	/* Input */
	input.Update();
	viewInputSystem->Update(deltaTime);

	/* Display */
	modelRenderSystem->Update(deltaTime);
	cameraSystem->Update(deltaTime);

	renderer.Update(deltaTime);
	world.CleanupEntities();
}

void ModelViewer::draw()
{
	renderer.Draw();
	window.NextFrame();
}