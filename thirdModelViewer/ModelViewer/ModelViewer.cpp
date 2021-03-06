#include "ModelViewer.h"

#include "Scene/DefaultScene.h"
#include <thirdEngine/Renderer/UI/ImGui/imgui_impl_glfw.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui_impl_opengl3.h>

ModelViewer::ModelViewer() :renderer(), uiRenderer(), world(), input(), window(input, renderer), eventManager(world)
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
	viewInputSystem = std::make_unique<ViewerInputSystem>(world, input, eventManager);
	inspectorSystem = std::make_unique<InspectorSystem>(world, renderer);

	SceneInfo sceneInfo;
	sceneInfo.world = &world;
	sceneInfo.renderer = &renderer;
	sceneInfo.uiRenderer = &uiRenderer;
	defaultScene = std::make_unique<TestScene>(sceneInfo, eventManager);
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
	inspectorSystem->Update(deltaTime);

	renderer.Update(deltaTime);
	uiRenderer.Update(deltaTime);
	world.CleanupEntities();
}

void ModelViewer::draw()
{
	//renderer.Draw();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	renderer.ClearBuffer();

	uiRenderer.Draw();
	window.NextFrame();
}