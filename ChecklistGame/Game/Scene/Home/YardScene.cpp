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
#include "Game/Event/LoadSceneEvent.h"
#include "Game/Extra/PrefabConstructionInfo.h"

#include "Game/Scene/Home/HouseScene.h"
#include "Game/Scene/Forest/ForestScene.h"
#include "Game/Scene/Town/TownScene.h"

#include "Game/Scene/SceneManager.h"

void YardScene::Setup()
{
	setupPrefab();

	entityId.player = world.ConstructPrefab(playerPrefab, World::NullEntity, new PrefabConstructionInfo(spawnPosition));
	entityId.camera = world.ConstructPrefab(cameraPrefab);

	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(entityId.player);
	playerComponent->data.camera = entityId.camera;

	entityId.storyboard = world.ConstructPrefab(storyboardPrefab);

	if (spawnPosition.GetWorldPosition() == glm::vec3(0))
	{
		CameraComponent* storyboardCamera = world.GetComponent<CameraComponent>(entityId.storyboard);
		storyboardCamera->isEnable = true;
		playerComponent->SetControlState(PlayerControlState::InGUI);
	}
	else
	{
		CameraComponent* camera = world.GetComponent<CameraComponent>(entityId.camera);
		camera->isEnable = true;
	}

	world.ConstructPrefab(yardPrefab);
	world.ConstructPrefab(skyboxPrefab);

	world.ConstructPrefab(brushInteractPrefab);
	world.ConstructPrefab(waterPotInteractPrefab);
	world.ConstructPrefab(doorInteractPrefab);
	world.ConstructPrefab(bikeInteractPrefab);

	for (int i = 0; i != 4; i++)
	{
		world.ConstructPrefab(flowerPrefab[i]);
	}
}

void YardScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	/* Yard */
	{
		lightingShader = shaderLoader.BuildFromFile("Shaders/lightingShader.vert", "Shaders/lightingShader.frag");
		//renderer.SetDirLight();

		glm::mat4 yardModelMat4(1.0f);
		yardModelMat4 *= glm::scale(yardModelMat4, glm::vec3(0.1f));

		yardModel = modelLoader.LoadFromFile("Resources/CG_HOME/NewHome.FBX", ModelLoadingPrefab::Optimize, yardModelMat4, true);

		DirLight dirLight;
		dirLight.direction = glm::vec3(-1, -1, 1);
		dirLight.position = glm::vec3(50, 50, -50);
		dirLight.ambient = glm::vec3(0.5, 0.5, 0.4);
		dirLight.diffuse = glm::vec3(0.7, 0.7, 0.75);
		dirLight.specular = glm::vec3(0.5, 0.5, 0.5);
		renderer.SetDirLight(dirLight);

		// Disable fence culling because it doesn't seems right
		yardModel.SetMeshCulling(74, false);

		Renderer::ModelHandle yardModelHandle = renderer.GetModelHandle(yardModel);
		yardPrefab.SetName("YardModel");

		yardPrefab.AddConstructor(new TransformComponentConstructor());
		yardPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, yardModelHandle, lightingShader));

		/* Fence */
		std::string paths[]{ "Resources/fobj_fence.png","Resources/fobj_fence2.png","Resources/fobj_fence3.png","Resources/fobj_fence4.png" };
		for (int i = 0; i != 4; i++)
		{
			fenceTexture[i] = textureLoader.LoadFromFile(TextureType::Diffuse, paths[i]);
		}

		b2BodyDef bodyDef;

		std::vector<b2FixtureDef> fixtures;
		b2FixtureDef fenceLeft;
		b2PolygonShape* s1 = new b2PolygonShape();
		s1->SetAsBox(0.2f, 27.0f, b2Vec2(26.5f, 1.0f), 0);
		fenceLeft.shape = s1;

		b2FixtureDef fenceFrontLeft;
		b2PolygonShape* s2 = new b2PolygonShape();
		s2->SetAsBox(7.35f, 0.2f, b2Vec2(19.15f, -28.0f), 0);
		fenceFrontLeft.shape = s2;

		b2FixtureDef fenceFrontRight;
		b2PolygonShape* s3 = new b2PolygonShape();
		s3->SetAsBox(7.35f, 0.2f, b2Vec2(-11.15f, -28.5f), 0);
		fenceFrontRight.shape = s3;

		b2FixtureDef fenceRight;
		b2PolygonShape* s4 = new b2PolygonShape();
		s4->SetAsBox(0.2f, 36.05f, b2Vec2(-18.5f, 7.55f), 0);
		fenceRight.shape = s4;

		b2FixtureDef house;
		b2PolygonShape* s5 = new b2PolygonShape();
		s5->SetAsBox(11.0f, 10.0f, b2Vec2(4.0f, 40.0f), 0);
		house.shape = s5;

		fixtures.push_back(fenceLeft);
		fixtures.push_back(fenceFrontLeft);
		fixtures.push_back(fenceFrontRight);
		fixtures.push_back(fenceRight);
		fixtures.push_back(house);

		yardPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, fixtures)));

		flowerModel = modelLoader.LoadFromFile("Resources/RainbowFlower/untitled.obj", ModelLoadingPrefab::Optimize, glm::mat4(0.0));
		Renderer::ModelHandle flowerModelHandle = renderer.GetModelHandle(flowerModel);

		Transform trans[4]{ glm::vec3(23.3105,1,17.2301),glm::vec3(23.4075,1,15.1081),glm::vec3(23.4075,1,19.5332),glm::vec3(23.2575,1,21.8582) };

		for (int i = 0; i != 4; i++)
		{
			flowerPrefab[i].SetName("Flower");
			flowerPrefab[i].AddConstructor(new TransformComponentConstructor(trans[i]));
			flowerPrefab[i].AddConstructor(new ModelRenderComponentConstructor(renderer, flowerModelHandle, lightingShader, false));
		}
	}

	interactEventResponder = std::make_shared<InteractEventResponder>(world, eventManager);

	/* Scene Interact Object */
	{
		b2BodyDef bodyDef;
		/* Brush */
		b2FixtureDef brush;
		brush.isSensor = true;
		b2PolygonShape* s1 = new b2PolygonShape();
		s1->SetAsBox(0.2f, 0.2f, b2Vec2(19.4f, -29.0f), 0);
		brush.shape = s1;

		brushInteractPrefab.SetName("Brush Interact");
		brushInteractPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, brush)));
		brushInteractPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(YardSceneBrushInteractEvent).hash_code())));

		/* Water Pot */
		b2FixtureDef waterPot;
		waterPot.isSensor = true;
		b2CircleShape* s2 = new b2CircleShape();
		s2->m_p = b2Vec2(18.69f, 25.63f);
		s2->m_radius = 0.2;
		waterPot.shape = s2;

		waterPotInteractPrefab.SetName("WaterPot Interact");
		waterPotInteractPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, waterPot)));
		waterPotInteractPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(YardSceneWaterPotInteractEvent).hash_code())));

		/* Door */
		b2FixtureDef door;
		door.isSensor = true;
		b2PolygonShape* s3 = new b2PolygonShape();
		s3->SetAsBox(4.5f, 0.2f, b2Vec2(5.0f, 26.3f), 0);
		door.shape = s3;

		doorInteractPrefab.SetName("Door Interact");
		doorInteractPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, door)));
		doorInteractPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(YardSceneDoorInteractEvent).hash_code())));

		/* Bike */
		b2FixtureDef bike;
		bike.isSensor = true;
		b2PolygonShape* s4 = new b2PolygonShape();
		s4->SetAsBox(1.0f, 3.0f, b2Vec2(-16.3f, -15.1f), 0);
		bike.shape = s4;

		bikeInteractPrefab.SetName("Bike Interact");
		bikeInteractPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, bike)));
		bikeInteractPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(YardSceneBikeInteractEvent).hash_code())));
	}

	/* Skybox */
	{
		Model skyboxModel = Box::GetSkybox("Resources/skybox/", std::vector<std::string>{"right.jpg", "left.jpg", "up.jpg", "down.jpg", "front.jpg", "back.jpg"});
		skyboxShader = shaderLoader.BuildFromFile("Shaders/skybox.vert", "Shaders/skybox.frag");

		Renderer::ModelHandle skyboxModelHandle = renderer.GetModelHandle(skyboxModel);

		skyboxPrefab.AddConstructor(new TransformComponentConstructor());
		skyboxPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, skyboxModelHandle, skyboxShader));
	}

	/* Player */
	{
		glm::mat4 playerModelMat4(1.0f);
		playerModelMat4 *= glm::scale(playerModelMat4, glm::vec3(0.5f));

		Model playerWalkModel = modelLoader.LoadFromFile("Resources/Walk.DAE", ModelLoadingPrefab::Default, playerModelMat4);
		Model playerPickLetterModel = modelLoader.LoadFromFile("Resources/PickLetter.DAE", ModelLoadingPrefab::Default, playerModelMat4);
		Model playerPutLetterModel = modelLoader.LoadFromFile("Resources/PutLetter.DAE", ModelLoadingPrefab::Default, playerModelMat4);
		Model playerTurnAroundModel = modelLoader.LoadFromFile("Resources/TurnAround.DAE", ModelLoadingPrefab::Default, playerModelMat4);

		Material i = playerWalkModel.GetMeshMaterial(0);
		i.SetProperty("texture_diffuse", MaterialProperty(globalVariable.clothes[globalVariable.clothIndex]));
		playerWalkModel.SetMeshMaterial(0, i);

		i = playerPickLetterModel.GetMeshMaterial(0);
		i.SetProperty("texture_diffuse", MaterialProperty(globalVariable.clothes[globalVariable.clothIndex]));
		playerPickLetterModel.SetMeshMaterial(0, i);

		i = playerPutLetterModel.GetMeshMaterial(0);
		i.SetProperty("texture_diffuse", MaterialProperty(globalVariable.clothes[globalVariable.clothIndex]));
		playerPutLetterModel.SetMeshMaterial(0, i);

		i = playerTurnAroundModel.GetMeshMaterial(0);
		i.SetProperty("texture_diffuse", MaterialProperty(globalVariable.clothes[globalVariable.clothIndex]));
		playerTurnAroundModel.SetMeshMaterial(0, i);

		playerPrefab.SetName("Player");
		playerPrefab.AddConstructor(new TransformComponentConstructor());

		PlayerComponent::Data playerData;
		playerData.walk = renderer.GetModelHandle(playerWalkModel);
		playerData.pickLetter = renderer.GetModelHandle(playerPickLetterModel);
		playerData.putLetter = renderer.GetModelHandle(playerPutLetterModel);
		playerData.turnAround = renderer.GetModelHandle(playerTurnAroundModel);

		playerPrefab.AddConstructor(new PlayerComponentConstructor(playerData));
		playerPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, playerData.walk, lightingShader, true));
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
		if (spawnPosition.GetWorldPosition() == glm::vec3(0)) {
			startMenu = std::make_shared<StartMenu>(eventManager);
			startMenuHandle = uiRenderer.GetEntityHandle(startMenu);
			letter = std::make_shared<Letter>(eventManager);
			letterHandle = uiRenderer.GetEntityHandle(letter);
		}

		/*
		checklist = std::make_shared<Checklist>(input, globalVariable);
		checklistHandle = uiRenderer.GetEntityHandle(checklist);
		*/

		brushLoading = std::make_shared<BrushLoading>(eventManager);
		brushLoadingHandle = uiRenderer.GetEntityHandle(brushLoading);
		brushSelector = std::make_shared<BrushSelector>(eventManager);
		brushSelectorHandle = uiRenderer.GetEntityHandle(brushSelector);
		waterFlower = std::make_shared<WaterFlower>(eventManager);
		waterFlowerHandle = uiRenderer.GetEntityHandle(waterFlower);

		bikeMap = std::make_shared<BikeMap>(eventManager, 1);
		bikeMapHandle = uiRenderer.GetEntityHandle(bikeMap);
	}

	/* Callback */
	{
#pragma region Start Scene
		if (spawnPosition.GetWorldPosition() == glm::vec3(0))
		{
			std::function<void(const GameStartEvent& event)> startGameCallback =
				[scene = this, entityId = &entityId, world = &world, storyboards = &storyboards](const GameStartEvent& event) {
				CollisionComponent* collision = world->GetComponent<CollisionComponent>(entityId->player);
				collision->controlMovement = false;
				StoryboardDirectorComponent* component = world->GetComponent<StoryboardDirectorComponent>(entityId->storyboard);
				StoryboardDirectorComponent::Data data((*storyboards)[0], std::unordered_map<std::string, eid_t>{ {"Camera", entityId->storyboard}, { "Player", entityId->player }});
#ifndef _DEBUG
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
				animElement.PositionChannel.push_back(KeyFrame<glm::vec3>(glm::vec3(9.0, 8.0, -49), sb.EndTime));

				/*
				animElement.RotationChannel.push_back(KeyFrame<glm::quat>(sbCameraTransfrorm->GetRotation(), 0.0f));
				animElement.RotationChannel.push_back(KeyFrame<glm::quat>(glm::quat(-0.0, 1.0, 0.0, -0.0), sb.EndTime));
				*/
				sb.AnimatedElementList.emplace("Camera", animElement);

				StoryboardDirectorComponent::Data data((*storyboards)[1], std::unordered_map<std::string, eid_t>{ {"Camera", entityId->storyboard}, { "Player", entityId->player }});

#ifndef _DEBUG
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
		}
#pragma endregion

#pragma region In Scene Interact

		std::function<void(const YardSceneBrushInteractEvent& event)> brushInteractCallback =
			[scene = this, brushSelector = brushSelector, &entityId = entityId, &world = world](const YardSceneBrushInteractEvent& event) {
			PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(entityId.player);
			playerComponent->SetControlState(PlayerControlState::InGUI);
			brushSelector->Show();
		};

		eventManager.RegisterForEvent<YardSceneBrushInteractEvent>(brushInteractCallback);

		std::function<void(const YardSceneBrushChangedEvent& event)> brushChangedCallback =
			[scene = this, brushLoading = brushLoading](const YardSceneBrushChangedEvent& event) {
			brushLoading->Show();
		};

		eventManager.RegisterForEvent<YardSceneBrushChangedEvent>(brushChangedCallback);

		std::function<void(const YardSceneBrushLoadingMidEvent& event)> brushLoadingMidCallback =
			[scene = this, &yardModel = yardModel, fenceTexture = fenceTexture, brushSelector = brushSelector](const YardSceneBrushLoadingMidEvent& event) {
			Material i = yardModel.GetMeshMaterial(74);
			i.SetProperty("texture_diffuse", MaterialProperty(fenceTexture[brushSelector->GetCurrentSelected()]));
			yardModel.SetMeshMaterial(74, i);
		};

		eventManager.RegisterForEvent<YardSceneBrushLoadingMidEvent>(brushLoadingMidCallback);

		std::function<void(const YardSceneBrushFinishEvent& event)> brushFinishCallback =
			[scene = this, &entityId = entityId, &world = world](const YardSceneBrushFinishEvent& event) {
			PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(entityId.player);
			playerComponent->SetControlState(PlayerControlState::Normal);
		};

		eventManager.RegisterForEvent<YardSceneBrushFinishEvent>(brushFinishCallback);

		std::function<void(const YardSceneWaterPotInteractEvent& event)> waterPotInteractCallback =
			[scene = this, waterFlower = waterFlower](const YardSceneWaterPotInteractEvent& event) {
			waterFlower->Show();
		};

		eventManager.RegisterForEvent<YardSceneWaterPotInteractEvent>(waterPotInteractCallback);

		std::function<void(const YardSceneWaterPotFinishEvent& event)> waterPotFinishCallback =
			[scene = this, &entityId = entityId, &world = world, &flowerModel = flowerModel](const YardSceneWaterPotFinishEvent& event) {
			PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(entityId.player);
			playerComponent->SetControlState(PlayerControlState::Normal);

			flowerModel.SetMeshTransform(0, 0, glm::scale(glm::mat4(1.0), glm::vec3(5.0)));
		};

		eventManager.RegisterForEvent<YardSceneWaterPotFinishEvent>(waterPotFinishCallback);

		std::function<void(const YardSceneDoorInteractEvent& event)> doorInteractCallback =
			[scene = this, &sceneManager = sceneManager](const YardSceneDoorInteractEvent& event) {
			LoadingScreenInfo info;
			info.LoopTime = 1.0f;
			info.ImageWidth = 400;
			info.ImageHeight = 400;
			info.LoadingImagePath = std::vector<std::string>{ "GUI/Loading/Key/0.png","GUI/Loading/Key/1.png","GUI/Loading/Key/2.png" };

			sceneManager.LoadScene<HouseScene>(info);
		};

		eventManager.RegisterForEvent<YardSceneDoorInteractEvent>(doorInteractCallback);

		std::function<void(const YardSceneBikeInteractEvent& event)> bikeInteractCallback =
			[scene = this, bikeMap = bikeMap](const YardSceneBikeInteractEvent& event) {
			bikeMap->Show();
		};

		eventManager.RegisterForEvent<YardSceneBikeInteractEvent>(bikeInteractCallback);

		std::function<void(const BikeMapChangeMapEvent& event)> bikeMapInteractCallback =
			[scene = this, &sceneManager = sceneManager](const BikeMapChangeMapEvent& event) {
			LoadingScreenInfo info;
			info.LoopTime = 1.0f;
			info.ImageWidth = 406;
			info.ImageHeight = 255;
			info.LoadingImagePath = std::vector<std::string>{ "GUI/Loading/Bicycle/0.png","GUI/Loading/Bicycle/1.png","GUI/Loading/Bicycle/2.png","GUI/Loading/Bicycle/3.png" };

			switch (event.dst)
			{
			case 0:
				sceneManager.LoadScene<ForestScene>(info);
				break;
			case 1:
				sceneManager.SetSpawnPosition(glm::vec3(-12.0143, 0, -16.5855));
				sceneManager.LoadScene<YardScene>(info);
				break;
			case 2:
				sceneManager.SetSpawnPosition(glm::vec3(-29.6974, 0, -119.804));
				sceneManager.LoadScene<TownScene>(info);
				break;
			default:
				break;
			}
		};

		eventManager.RegisterForEvent<BikeMapChangeMapEvent>(bikeMapInteractCallback);

#pragma endregion
	}

	prefabsSteup = true;
}

void YardScene::Finish()
{
	glEnable(GL_CULL_FACE);
	renderer.GenVAO();
}

void YardScene::PreDestruct()
{
	glDisable(GL_CULL_FACE);
	eventManager.ClearEventListener<GameStartEvent>();
	eventManager.ClearEventListener<YardSceneOpenSceneStoryboardEndEvent>();
	eventManager.ClearEventListener<YardSceneLetterCloseEvent>();
	eventManager.ClearEventListener<YardSceneTurnAroundStoryboardEndEvent>();
	eventManager.ClearEventListener<YardSceneBrushInteractEvent>();
	eventManager.ClearEventListener<YardSceneWaterPotInteractEvent>();
	eventManager.ClearEventListener<YardSceneDoorInteractEvent>();
}