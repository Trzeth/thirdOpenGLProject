#include "YardScene.h"

#include <thirdEngine/Renderer/Box.h>

#include "Game/Component/TransformComponent.h"
#include "Game/Component/ModelRenderComponent.h"
#include "Game/Component/CameraComponent.h"
#include "Game/Component/PlayerComponent.h"
#include "Game/Component/StoryboardDirectorComponent.h"
#include "Game/Component/CollisionComponent.h"
#include "Game/Component/RigidbodyMotorComponent.h"
#include "Game/Component/InteractComponent.h"

#include "Game/Event/YardSceneEvent.h"
#include "Game/Event/GameEvent.h"

#include "Game/Scene/Home/HouseScene.h"

#include "Game/Scene/SceneManager.h"

void YardScene::Setup()
{
	setupPrefab();

	entityId.player = world.ConstructPrefab(playerPrefab);
	entityId.camera = world.ConstructPrefab(cameraPrefab);

	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(entityId.player);
	playerComponent->data.camera = entityId.camera;
	playerComponent->SetControlState(PlayerControlState::InGUI);

	Camera* cam = &world.GetComponent<CameraComponent>(entityId.camera)->data;

	entityId.storyboard = world.ConstructPrefab(storyboardPrefab);

	CameraComponent* storyboardCamera = world.GetComponent<CameraComponent>(entityId.storyboard);
	storyboardCamera->isEnable = true;

	world.ConstructPrefab(yardPrefab);
	world.ConstructPrefab(skyboxPrefab);

	world.ConstructPrefab(brushInteractPrefab);
	world.ConstructPrefab(waterPotInteractPrefab);
	world.ConstructPrefab(doorInteractPrefab);
}

void YardScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	/* Yard */
	{
		plainShader = shaderLoader.BuildFromFile("Shaders/plainShader.vs", "Shaders/plainShader.fs");

		glm::mat4 yardModelMat4(1.0f);
		yardModelMat4 *= glm::scale(yardModelMat4, glm::vec3(0.1f));

		Model yardModel = modelLoader.LoadFromFile("Resources/CG_HOME/NewHome.FBX", ModelLoadingPrefab::Optimize, yardModelMat4, true);
		yardModel.SetMeshCulling(74, false);
		Renderer::ModelHandle yardModelHandle = renderer.GetModelHandle(yardModel);
		yardPrefab.SetName("YardModel");

		yardPrefab.AddConstructor(new TransformComponentConstructor());
		yardPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, yardModelHandle, plainShader));

		b2BodyDef bodyDef;

		std::vector<b2FixtureDef> fixtures;
		b2FixtureDef fenceLeft;
		b2PolygonShape* s1 = new b2PolygonShape();
		s1->SetAsBox(0.2f, 27.0f, b2Vec2(-26.5f, -1.0f), 0);
		fenceLeft.shape = s1;

		b2FixtureDef fenceFrontLeft;
		b2PolygonShape* s2 = new b2PolygonShape();
		s2->SetAsBox(7.35f, 0.2f, b2Vec2(-19.15f, 28.0f), 0);
		fenceFrontLeft.shape = s2;

		b2FixtureDef fenceFrontRight;
		b2PolygonShape* s3 = new b2PolygonShape();
		s3->SetAsBox(7.35f, 0.2f, b2Vec2(11.15f, 28.5f), 0);
		fenceFrontRight.shape = s3;

		b2FixtureDef fenceRight;
		b2PolygonShape* s4 = new b2PolygonShape();
		s4->SetAsBox(0.2f, 36.05f, b2Vec2(18.5f, -7.55f), 0);
		fenceRight.shape = s4;

		b2FixtureDef house;
		b2PolygonShape* s5 = new b2PolygonShape();
		s5->SetAsBox(11.0f, 10.0f, b2Vec2(-4.0f, -40.0f), 0);
		house.shape = s5;

		b2FixtureDef tree1;
		b2CircleShape* s6 = new b2CircleShape();
		s6->m_p = b2Vec2(-14.5f, 15.1f);
		s6->m_radius = 2;
		tree1.shape = s6;

		b2FixtureDef tree2;
		b2CircleShape* s7 = new b2CircleShape();
		s7->m_p = b2Vec2(7.2f, -0.2f);
		s7->m_radius = 2;
		tree2.shape = s7;

		fixtures.push_back(fenceLeft);
		fixtures.push_back(fenceFrontLeft);
		fixtures.push_back(fenceFrontRight);
		fixtures.push_back(fenceRight);
		fixtures.push_back(house);
		fixtures.push_back(tree1);
		fixtures.push_back(tree2);

		//yardPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, fixtures)));
	}

	interactEventResponder = std::make_shared<InteractEventResponder>(world, eventManager);

	/* Scene Interact Object */
	{
		/* Brush */
		b2BodyDef brushDef;

		b2FixtureDef brush;
		brush.isSensor = true;
		b2PolygonShape* s1 = new b2PolygonShape();
		s1->SetAsBox(0.2f, 0.2f, b2Vec2(-19.4f, 29.0f), 0);
		brush.shape = s1;

		brushInteractPrefab.SetName("Brush Interact");
		brushInteractPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(brushDef, brush)));
		brushInteractPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(YardSceneBrushInteractEvent).hash_code())));

		/* Water Pot */
		b2BodyDef waterPotDef;

		b2FixtureDef waterPot;
		waterPot.isSensor = true;
		b2CircleShape* s2 = new b2CircleShape();
		s2->m_p = b2Vec2(-18.69f, -25.63f);
		s2->m_radius = 0.2;
		waterPot.shape = s2;

		waterPotInteractPrefab.SetName("WaterPot Interact");
		waterPotInteractPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(waterPotDef, waterPot)));
		waterPotInteractPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(YardSceneWaterPotInteractEvent).hash_code())));

		/* Door */
		b2BodyDef doorDef;

		b2FixtureDef door;
		door.isSensor = true;
		b2PolygonShape* s3 = new b2PolygonShape();
		s3->SetAsBox(4.5f, 0.2f, b2Vec2(5.0f, 26.3f), 0);
		door.shape = s3;

		doorInteractPrefab.SetName("WaterPot Interact");
		doorInteractPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(doorDef, door)));
		doorInteractPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(YardSceneDoorInteractEvent).hash_code())));
	}

	/* Skybox */
	{
		Model skyboxModel = Box::GetSkybox("Resources/skybox/", std::vector<std::string>{"right.jpg", "left.jpg", "up.jpg", "down.jpg", "front.jpg", "back.jpg"});
		skyboxShader = shaderLoader.BuildFromFile("Shaders/skybox.vs", "Shaders/skybox.fs");

		Renderer::ModelHandle skyboxModelHandle = renderer.GetModelHandle(skyboxModel);

		skyboxPrefab.AddConstructor(new TransformComponentConstructor());
		skyboxPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, skyboxModelHandle, skyboxShader));
	}

	/* Player */
	{
		skinnedShader = shaderLoader.BuildFromFile("Shaders/skinnedShader.vs", "Shaders/skinnedShader.fs");

		glm::mat4 playerModelMat4(1.0f);
		playerModelMat4 *= glm::scale(playerModelMat4, glm::vec3(0.5f));

		Model playerWalkModel = modelLoader.LoadFromFile("Resources/Walk.DAE", ModelLoadingPrefab::Default, playerModelMat4);
		Model playerPickLetterModel = modelLoader.LoadFromFile("Resources/PickLetter.DAE", ModelLoadingPrefab::Default, playerModelMat4);
		Model playerPutLetterModel = modelLoader.LoadFromFile("Resources/PutLetter.DAE", ModelLoadingPrefab::Default, playerModelMat4);
		Model playerTurnAroundModel = modelLoader.LoadFromFile("Resources/TurnAround.DAE", ModelLoadingPrefab::Default, playerModelMat4);

		playerPrefab.SetName("Player");
		playerPrefab.AddConstructor(new TransformComponentConstructor());

		PlayerComponent::Data playerData;
		playerData.walk = renderer.GetModelHandle(playerWalkModel);
		playerData.pickLetter = renderer.GetModelHandle(playerPickLetterModel);
		playerData.putLetter = renderer.GetModelHandle(playerPutLetterModel);
		playerData.turnAround = renderer.GetModelHandle(playerTurnAroundModel);

		playerPrefab.AddConstructor(new PlayerComponentConstructor(playerData));
		playerPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, playerData.walk, skinnedShader));
		b2BodyDef bodyDef;
		bodyDef.type = b2BodyType::b2_dynamicBody;
		bodyDef.allowSleep = false;

		b2FixtureDef fixtureDef;
		b2CircleShape* shape = new b2CircleShape();
		shape->m_radius = 2.0f;
		fixtureDef.shape = shape;
		fixtureDef.density = 0.1f;
		fixtureDef.friction = 0.3f;

		playerPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, fixtureDef)));
		playerPrefab.AddConstructor(new RigidbodyMotorComponentConstructor(RigidbodyMotorComponent::Data()));
	}

	/* Camera */
	{
		cameraPrefab.AddConstructor(new TransformComponentConstructor());
		cameraPrefab.AddConstructor(new CameraComponentConstructor(CameraComponent::Data()));
	}

	/* Storyboard */
	{
		storyboards = storyboardLoader.LoadFromFile("Storyboards/Yard.thirdsbs");

		storyboardPrefab.AddConstructor(new StoryboardDirectorComponentConstructor(StoryboardDirectorComponent::Data()));
		storyboardPrefab.AddConstructor(new CameraComponentConstructor(CameraComponent::Data()));

		// First Position
		Transform storyboardCameraTransform;
		storyboardCameraTransform.SetPosition(storyboards[0].AnimatedElementList["Camera"].PositionChannel[0].value);
		storyboardCameraTransform.SetRotation(storyboards[0].AnimatedElementList["Camera"].RotationChannel[0].value);
		storyboardPrefab.AddConstructor(new TransformComponentConstructor(storyboardCameraTransform));
	}

	/* UI */
	{
		startMenu = std::make_shared<StartMenu>(eventManager);
		startMenuHandle = uiRenderer.GetEntityHandle(startMenu);
		letter = std::make_shared<Letter>(eventManager);
		letterHandle = uiRenderer.GetEntityHandle(letter);
		checklist = std::make_shared<Checklist>(input, globalVariable);
		checklistHandle = uiRenderer.GetEntityHandle(checklist);
	}

	/* Callback */
	{
#pragma region Start Scene

		std::function<void(const GameStartEvent& event)> startGameCallback =
			[scene = this, entityId = &entityId, world = &world, storyboards = &storyboards](const GameStartEvent& event) {
			CollisionComponent* collision = world->GetComponent<CollisionComponent>(entityId->player);
			collision->controlMovement = false;
			StoryboardDirectorComponent* component = world->GetComponent<StoryboardDirectorComponent>(entityId->storyboard);
			StoryboardDirectorComponent::Data data((*storyboards)[0], std::unordered_map<std::string, eid_t>{ {"Camera", entityId->storyboard}, { "Player", entityId->player }});
#ifdef _DEBUG
			component->BeginStoryboard<YardSceneOpenSceneStoryboardEndEvent>(data, (*storyboards)[0].EndTime);
#else
			component->BeginStoryboard<YardSceneOpenSceneStoryboardEndEvent>(data);
#endif // DEBUG
		};

		eventManager.RegisterForEvent<GameStartEvent>(startGameCallback);

		std::function<void(const YardSceneOpenSceneStoryboardEndEvent& event)> openSceneSBEndCallback =
			[scene = this, letter = letter](const YardSceneOpenSceneStoryboardEndEvent& event) {
			letter->Show();
		};

		eventManager.RegisterForEvent<YardSceneOpenSceneStoryboardEndEvent>(openSceneSBEndCallback);

		std::function<void(const YardSceneLetterCloseEvent& event)> letterCloseCallback =
			[scene = this, entityId = &entityId, world = &world, storyboards = &storyboards](const YardSceneLetterCloseEvent& event) {
			StoryboardDirectorComponent* component = world->GetComponent<StoryboardDirectorComponent>(entityId->storyboard);
			auto sbCameraTransfrorm = world->GetComponent<TransformComponent>(entityId->storyboard)->data;

			Storyboard::AnimatedElement animElement;
			auto& sb = (*storyboards)[1];

			animElement.PositionChannel.push_back(KeyFrame<glm::vec3>(sbCameraTransfrorm->GetPosition(), 0.0f));
			animElement.PositionChannel.push_back(KeyFrame<glm::vec3>(glm::vec3(-9.554, 15.607, 41.592), sb.EndTime));

			animElement.RotationChannel.push_back(KeyFrame<glm::quat>(sbCameraTransfrorm->GetRotation(), 0.0f));
			animElement.RotationChannel.push_back(KeyFrame<glm::quat>(glm::quat(0.9239, -0.382683, 0.0, 0.0), sb.EndTime));

			sb.AnimatedElementList.emplace("Camera", animElement);

			StoryboardDirectorComponent::Data data((*storyboards)[1], std::unordered_map<std::string, eid_t>{ {"Camera", entityId->storyboard}, { "Player", entityId->player }});

#ifdef _DEBUG
			component->BeginStoryboard<YardSceneTurnAroundStoryboardEndEvent>(data, (*storyboards)[1].EndTime);
#else
			component->BeginStoryboard<YardSceneTurnAroundStoryboardEndEvent>(data);
#endif // DEBUG
		};

		eventManager.RegisterForEvent<YardSceneLetterCloseEvent>(letterCloseCallback);

		std::function<void(const YardSceneTurnAroundStoryboardEndEvent& event)> turnAroundSBEndCallback =
			[scene = this, entityId = &entityId, world = &world, storyboards = &storyboards](const YardSceneTurnAroundStoryboardEndEvent& event) {
			CollisionComponent* collision = world->GetComponent<CollisionComponent>(entityId->player);
			collision->controlMovement = true;

			PlayerComponent* playerComponent = world->GetComponent<PlayerComponent>(entityId->player);
			playerComponent->SetControlState(PlayerControlState::Normal);

			CameraComponent* storyboardCamera = world->GetComponent<CameraComponent>(entityId->storyboard);
			storyboardCamera->isEnable = false;

			CameraComponent* camera = world->GetComponent<CameraComponent>(entityId->camera);
			camera->isEnable = true;
		};

		eventManager.RegisterForEvent<YardSceneTurnAroundStoryboardEndEvent>(turnAroundSBEndCallback);

#pragma endregion

#pragma region In Scene Interact

		std::function<void(const YardSceneBrushInteractEvent& event)> brushInteractCallback =
			[scene = this, &sceneManager = sceneManager](const YardSceneBrushInteractEvent& event) {
			printf("Brush Interact.\n");
		};

		eventManager.RegisterForEvent<YardSceneBrushInteractEvent>(brushInteractCallback);

		std::function<void(const YardSceneWaterPotInteractEvent& event)> waterPotInteractCallback =
			[scene = this, &sceneManager = sceneManager](const YardSceneWaterPotInteractEvent& event) {
			printf("Waterpot Interact.\n");
		};

		eventManager.RegisterForEvent<YardSceneWaterPotInteractEvent>(waterPotInteractCallback);

		std::function<void(const YardSceneDoorInteractEvent& event)> doorInteractCallback =
			[scene = this, &sceneManager = sceneManager](const YardSceneDoorInteractEvent& event) {
			LoadingScreenInfo info;
			info.LoopTime = 1.0f;
			info.LoadingImagePath = std::vector<std::string>{ "GUI/Loading0.png" };

			sceneManager.LoadScene<HouseScene>(info);
		};

		eventManager.RegisterForEvent<YardSceneDoorInteractEvent>(doorInteractCallback);

#pragma endregion
	}

	prefabsSteup = true;
}

void YardScene::Finish()
{
	renderer.GenVAO();
}

void YardScene::PreDestruct()
{
	eventManager.ClearEventListener<GameStartEvent>();
	eventManager.ClearEventListener<YardSceneOpenSceneStoryboardEndEvent>();
	eventManager.ClearEventListener<YardSceneLetterCloseEvent>();
	eventManager.ClearEventListener<YardSceneTurnAroundStoryboardEndEvent>();
	eventManager.ClearEventListener<YardSceneBrushInteractEvent>();
	eventManager.ClearEventListener<YardSceneWaterPotInteractEvent>();
	eventManager.ClearEventListener<YardSceneDoorInteractEvent>();
}