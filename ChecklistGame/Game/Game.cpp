#include "Game.h"

#include "Component/PlayerComponent.h"
#include "Component/CameraComponent.h"

#include "Event/LoadSceneStartEvent.h"
#include "Event/LoadSceneEndEvent.h"

#include "Scene/Home/YardScene.h"
Game::Game() :
	renderer(), uiRenderer(), world(), input(),
	window(input, renderer), eventManager(world),
	dynamicsWorld(new b2World(b2Vec2(0, 0))), physics(dynamicsWorld.get(), eventManager), debugDrawer(),
	globalVariable(4)
{
	restart = false;
	running = false;
	wireframe = false;
	started = false;
	loadingScene = false;
	loadingSceneEnd = false;

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

	/* Renderer*/
	renderer.Initialize(window.GetWidth(), window.GetHeight());
	uiRenderer.Initialize(window.GetWindow());

	/* Physics */
	debugDrawer.Initialize();
	debugDrawer.SetFlags(b2Draw::e_shapeBit);
	dynamicsWorld->SetDebugDraw(&debugDrawer);

	/* System */
	modelRenderSystem = std::make_unique<ModelRenderSystem>(world, renderer);
	cameraSystem = std::make_unique<CameraSystem>(world, renderer);
	playerInputSystem = std::make_unique<PlayerInputSystem>(world, input, eventManager);
	playerAnimationStateSystem = std::make_unique<PlayerAnimationStateSystem>(world, renderer);
	playerControlStateSystem = std::make_unique<PlayerControlStateSystem>(world, window);
	storyboardDirectorSystem = std::make_unique<StoryboardDirectorSystem>(world, eventManager);
	collisionUpdateSystem = std::make_unique<CollisionUpdateSystem>(world);
	rigidbodyMotorSystem = std::make_unique<RigidbodyMotorSystem>(world);

	/* Scene Manager */
	SceneInfo sceneInfo;
	sceneInfo.world = &world;
	sceneInfo.renderer = &renderer;
	sceneInfo.uiRenderer = &uiRenderer;
	sceneInfo.dynamicsWorld = dynamicsWorld.get();
	sceneInfo.globalVariable = &globalVariable;
	sceneInfo.input = &input;
	sceneInfo.eventManager = &eventManager;

	sceneManager = std::make_unique<SceneManager>(window, sceneInfo);

	std::function<void(const LoadSceneStartEvent& event)> LoadSceneStartCallback =
		[&loadingScene = loadingScene](const LoadSceneStartEvent& event) {
		loadingScene = true;
	};
	eventManager.RegisterForEvent<LoadSceneStartEvent>(LoadSceneStartCallback);

	std::function<void(const LoadSceneEndEvent& event)> LoadSceneEndCallback =
		[&loadingScene = loadingScene, &loadingSceneEnd = loadingSceneEnd, &sceneManager = sceneManager, &world = world, &debugDrawer = debugDrawer](const LoadSceneEndEvent& event) {
		loadingSceneEnd = true;
	};
	eventManager.RegisterForEvent<LoadSceneEndEvent>(LoadSceneEndCallback);

	sceneManager->LoadScene<YardScene>(LoadingScreenInfo(
		std::vector<std::string>{"GUI/Loading0.png", "GUI/Loading1.png", "GUI/Loading2.png", "GUI/Loading3.png", "GUI/Loading4.png", "GUI/Loading5.png"}, 0.5));

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
	/* SceneManager */
	if (loadingScene)
	{
		sceneManager->Update(deltaTime);

		if (loadingSceneEnd)
		{
			sceneManager->LoadFinish();
			loadingScene = false;

			std::vector<eid_t> playerEntities = world.GetEntitiesWithComponent<PlayerComponent>();
			if (playerEntities.size() < 0)
			{
				printf("WARNING: No camera in scene");
			}
			else
			{
				PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(playerEntities[0]);
				CameraComponent* cameraComponent = world.GetComponent<CameraComponent>(playerComponent->data.camera);
				debugDrawer.SetCamera(&cameraComponent->data);
			}

			loadingScene = false;
			loadingSceneEnd = false;
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
	physics.Update(deltaTime);

	/* Display */
	storyboardDirectorSystem->Update(deltaTime);
	playerAnimationStateSystem->Update(deltaTime);
	playerControlStateSystem->Update(deltaTime);
	collisionUpdateSystem->Update(deltaTime);

	modelRenderSystem->Update(deltaTime);
	cameraSystem->Update(deltaTime);

	renderer.Update(deltaTime);
	uiRenderer.Update(deltaTime);

	world.CleanupEntities();
}

void Game::draw()
{
	if (loadingScene)
	{
		sceneManager->Draw();
		window.NextFrame();
		return;
	}

	renderer.Draw();
	dynamicsWorld->DebugDraw();
	debugDrawer.Draw();
	debugDrawer.Reset();
	uiRenderer.Draw();
	window.NextFrame();
}