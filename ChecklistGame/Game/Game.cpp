#include "Game.h"

#include "Scene/Home/YardScene.h"
#include "Component/PlayerComponent.h"
#include "Component/CameraComponent.h"

Game::Game() :
	renderer(), uiRenderer(), world(), input(),
	window(input, renderer), eventManager(world),
	dynamicsWorld(new b2World(b2Vec2())), physics(dynamicsWorld.get(), eventManager), debugDrawer()
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

	SceneInfo sceneInfo;
	sceneInfo.world = &world;
	sceneInfo.renderer = &renderer;
	sceneInfo.uiRenderer = &uiRenderer;
	sceneInfo.dynamicsWorld = dynamicsWorld.get();
	defaultScene = std::make_unique<YardScene>(sceneInfo, eventManager);
	defaultScene->Setup();

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
	renderer.Draw();
	dynamicsWorld->DebugDraw();
	debugDrawer.Draw();
	debugDrawer.Reset();
	uiRenderer.Draw();
	window.NextFrame();
}