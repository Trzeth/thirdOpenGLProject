#include "Game.h"

#include "Component/PlayerComponent.h"
#include "Component/CameraComponent.h"

#include "Event/LoadSceneEvent.h"

#include "Scene/Home/YardScene.h"
#include "Scene/Town/TownScene.h"
#include "Scene/Forest/ForestScene.h"
#include "Scene/Town/ShopScene.h"
#include "Scene/Town/ClothStoreScene.h"
#include "Scene/Home/HouseScene.h"

#include <thirdEngine/Renderer/UI/ImGui/imgui.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui_impl_glfw.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui_impl_opengl3.h>

Game::Game() :
	renderer(), uiRenderer(), world(), input(),
	window(input, renderer), eventManager(world),
	dynamicsWorld(new b2World(b2Vec2(0, 0))), physics(dynamicsWorld.get(), eventManager), debugDrawer(),
	globalVariable(6)
{
	restart = false;
	running = false;
	wireframe = false;
	started = false;
	loadSceneWait = false;
	loadScene = false;
	loadSceneEnd = false;

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

	input.SetDefaultMapping("E", KbmAxis_E, KbmAxis_None);
	input.SetDefaultMapping("LeftMouse", KbmAxis_MouseLeft, KbmAxis_None);
	input.SetDefaultMapping("R", KbmAxis_R, KbmAxis_None);
	input.SetDefaultMapping("Return", KbmAxis_Return, KbmAxis_None);

	/* Renderer*/
	renderer.Initialize(window.GetWidth(), window.GetHeight());
	uiRenderer.Initialize(window.GetWindow());

	/* Physics */
	debugDrawer.Initialize();
	debugDrawer.SetFlags(b2Draw::e_shapeBit);
	dynamicsWorld->SetDebugDraw(&debugDrawer);
	dynamicsWorld->SetContactListener(&physics);

	/* System */
	modelRenderSystem = std::make_unique<ModelRenderSystem>(world, renderer);
	cameraSystem = std::make_unique<CameraSystem>(world, renderer);
	playerInputSystem = std::make_unique<PlayerInputSystem>(world, input, eventManager);
	playerAnimationStateSystem = std::make_unique<PlayerAnimationStateSystem>(world, renderer);
	playerControlStateSystem = std::make_unique<PlayerControlStateSystem>(world, window);
	storyboardDirectorSystem = std::make_unique<StoryboardDirectorSystem>(world, eventManager);
	collisionUpdateSystem = std::make_unique<CollisionUpdateSystem>(world);
	rigidbodyMotorSystem = std::make_unique<RigidbodyMotorSystem>(world);
	characterShowcaseSystem = std::make_unique<CharacterShowcaseSystem>(world, input, renderer);

	/* Scene Manager */
	SceneInfo sceneInfo;
	sceneInfo.world = &world;
	sceneInfo.renderer = &renderer;
	sceneInfo.uiRenderer = &uiRenderer;
	sceneInfo.dynamicsWorld = dynamicsWorld.get();
	sceneInfo.globalVariable = &globalVariable;

	std::vector<Texture> clothes;
	TextureLoader textureLoader;
	std::string paths[]{ "Resources/Cloth/0.png","Resources/Cloth/1.png","Resources/Cloth/2.png","Resources/Cloth/3.png" };
	for (int i = 0; i != 4; i++)
	{
		clothes.push_back(textureLoader.LoadFromFile(TextureType::Diffuse, paths[i]));
	}

	sceneInfo.globalVariable->clothes = clothes;
	sceneInfo.input = &input;
	sceneInfo.eventManager = &eventManager;
	sceneInfo.spawnPosition = Transform();

	sceneManager = std::make_unique<SceneManager>(window, sceneInfo);

	std::function<void(const LoadSceneWaitEvent& event)> LoadSceneWaitCallback =
		[&loadSceneWait = loadSceneWait](const LoadSceneWaitEvent& event) {
		loadSceneWait = true;
	};
	eventManager.RegisterForEvent<LoadSceneWaitEvent>(LoadSceneWaitCallback);

	std::function<void(const LoadSceneEndEvent& event)> LoadSceneEndCallback =
		[&loadSceneEnd = loadSceneEnd](const LoadSceneEndEvent& event) {
		loadSceneEnd = true;
	};
	eventManager.RegisterForEvent<LoadSceneEndEvent>(LoadSceneEndCallback);

	sceneManager->LoadScene<ForestScene>(LoadingScreenInfo(
		std::vector<std::string>{"GUI/Loading/Checklist/0.png", "GUI/Loading/Checklist/1.png", "GUI/Loading/Checklist/2.png", "GUI/Loading/Checklist/3.png", "GUI/Loading/Checklist/4.png", "GUI/Loading/Checklist/5.png"}, 400, 583, 0.5));

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
	if (deltaTime > FRAMEDURATION)
		deltaTime = FRAMEDURATION;

	/* SceneManager */
	if (loadSceneWait || loadScene || loadSceneEnd)
	{
		if (loadSceneWait)
		{
			sceneManager->LoadBegin();
			loadSceneWait = false;
			loadScene = true;
		}

		if (loadScene) {
			sceneManager->Update(deltaTime);
		}

		if (loadSceneEnd) {
			loadScene = false;

			sceneManager->LoadFinish();
			loadSceneEnd = false;

			std::vector<eid_t> playerEntities = world.GetEntitiesWithComponent<PlayerComponent>();
			if (playerEntities.size() <= 0)
			{
				printf("WARNING: No camera in scene");
			}
			else
			{
				PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(playerEntities[0]);
				CameraComponent* cameraComponent = world.GetComponent<CameraComponent>(playerComponent->data.camera);
				debugDrawer.SetCamera(&cameraComponent->data);
			}
		}

		return;
	}

	/* Time */
	globalVariable.currentTime += deltaTime;

	/* Input */
	input.Update();
	playerInputSystem->Update(deltaTime);

	/* Physics */
	rigidbodyMotorSystem->Update(deltaTime);
	dynamicsWorld->Step(deltaTime, VELOCITYITERATION, POSITIONITERATION);

	/* Display */
	storyboardDirectorSystem->Update(deltaTime);
	playerAnimationStateSystem->Update(deltaTime);
	playerControlStateSystem->Update(deltaTime);
	collisionUpdateSystem->Update(deltaTime);
	characterShowcaseSystem->Update(deltaTime);

	modelRenderSystem->Update(deltaTime);
	cameraSystem->Update(deltaTime);

	renderer.Update(deltaTime);
	uiRenderer.Update(deltaTime);

	world.CleanupEntities();
}

void Game::draw()
{
	if (loadScene)
	{
		sceneManager->Draw();
		window.NextFrame();
		return;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	renderer.Draw();
	//dynamicsWorld->DebugDraw();
	debugDrawer.Draw();
	debugDrawer.Reset();
	uiRenderer.Draw();
	window.NextFrame();
}