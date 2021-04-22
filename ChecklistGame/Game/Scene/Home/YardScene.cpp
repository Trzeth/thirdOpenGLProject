#include "YardScene.h"

#include <thirdEngine/Renderer/Box.h>

#include "Game/Component/TransformComponent.h"
#include "Game/Component/ModelRenderComponent.h"
#include "Game/Component/CameraComponent.h"
#include "Game/Component/PlayerComponent.h"
#include "Game/Component/StoryboardDirectorComponent.h"
#include "Game/Component/CollisionComponent.h"
#include "Game/Component/RigidbodyMotorComponent.h"

#include "Game/Event/GameStartEvent.h"
#include "Game/Event/YardSceneOpenSceneStoryboardEndEvent.h"
#include "Game/Event/YardSceneTurnAroundStoryboardEndEvent.h"
#include "Game/Event/YardSceneLetterCloseEvent.h"

#include "Game/Extra/PrefabConstructionInfo.h"

void YardScene::Setup()
{
	setupPrefab();

	PrefabConstructionInfo playerInfo = PrefabConstructionInfo(Transform(glm::vec3(0, 0, 0)));
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
}

void YardScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	skinnedShader = shaderLoader.BuildFromFile("Shaders/skinnedShader.vs", "Shaders/skinnedShader.fs");
	plainShader = shaderLoader.BuildFromFile("Shaders/plainShader.vs", "Shaders/plainShader.fs");

	/* Yard */
	glm::mat4 yardModelMat4(1.0f);
	yardModelMat4 *= glm::scale(yardModelMat4, glm::vec3(0.1f));
	yardModelMat4 *= glm::mat4_cast(glm::angleAxis(glm::radians(180.0f), Transform::UP));

	Model yardModel = modelLoader.LoadModel("Resources/Scene/Scene_1_3.FBX", yardModelMat4);
	yardModel.GenVAO();

	Renderer::ModelHandle yardModelHandle = renderer.GetModelHandle(yardModel);
	yardPrefab.SetName("YardModel");

	yardPrefab.AddConstructor(new TransformComponentConstructor());
	yardPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, yardModelHandle, plainShader));

	/* Skybox */
	Model skyboxModel = Box::GetSkybox("Resources/skybox/", std::vector<std::string>{"right.jpg", "left.jpg", "up.jpg", "down.jpg", "front.jpg", "back.jpg"});
	skyboxModel.GenVAO();
	skyboxShader = shaderLoader.BuildFromFile("Shaders/skybox.vs", "Shaders/skybox.fs");

	skybox = renderer.GetRenderableHandle(renderer.GetModelHandle(skyboxModel), skyboxShader);

	/* Player */
	glm::mat4 playerModelMat4(1.0f);
	playerModelMat4 *= glm::scale(playerModelMat4, glm::vec3(0.5f));
	playerModelMat4 *= glm::mat4_cast(glm::angleAxis(glm::radians(180.0f), Transform::UP));

	Model playerWalkModel = modelLoader.LoadModel("Resources/Walk.DAE", playerModelMat4);
	Model playerPickLetterModel = modelLoader.LoadModel("Resources/PickLetter.DAE", playerModelMat4);
	Model playerPutLetterModel = modelLoader.LoadModel("Resources/PutLetter.DAE", playerModelMat4);
	Model playerTurnAroundModel = modelLoader.LoadModel("Resources/TurnAround.DAE", playerModelMat4);

	playerWalkModel.GenVAO();
	playerPickLetterModel.GenVAO();
	playerPutLetterModel.GenVAO();
	playerTurnAroundModel.GenVAO();

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

	playerPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, std::vector<b2FixtureDef>{fixtureDef})));
	playerPrefab.AddConstructor(new RigidbodyMotorComponentConstructor(RigidbodyMotorComponent::Data()));
	/* Camera */
	cameraPrefab.AddConstructor(new TransformComponentConstructor());
	cameraPrefab.AddConstructor(new CameraComponentConstructor(CameraComponent::Data()));

	/* Storyboard */
	storyboards = storyboardLoader.LoadFromFile("Storyboards/Yard.thirdsbs");

	storyboardPrefab.AddConstructor(new StoryboardDirectorComponentConstructor(StoryboardDirectorComponent::Data()));
	storyboardPrefab.AddConstructor(new CameraComponentConstructor(CameraComponent::Data()));

	// First Position
	Transform storyboardCameraTransform;
	storyboardCameraTransform.SetPosition(storyboards[0].AnimatedElementList["Camera"].PositionChannel[0].value);
	storyboardCameraTransform.SetRotation(storyboards[0].AnimatedElementList["Camera"].RotationChannel[0].value);
	storyboardPrefab.AddConstructor(new TransformComponentConstructor(storyboardCameraTransform));

	/* UI */
	startMenu = std::make_shared<StartMenu>(eventManager);
	startMenuHandle = uiRenderer.GetEntityHandle(startMenu);
	letter = std::make_shared<Letter>(eventManager);
	letterHandle = uiRenderer.GetEntityHandle(letter);

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
		animElement.PositionChannel.push_back(KeyFrame<glm::vec3>(glm::vec3(-9.554, 15.607, 41.592), sb.EndTime));

		animElement.RotationChannel.push_back(KeyFrame<glm::quat>(sbCameraTransfrorm->GetRotation(), 0.0f));
		animElement.RotationChannel.push_back(KeyFrame<glm::quat>(glm::quat(0.9239, -0.382683, 0.0, 0.0), sb.EndTime));

		sb.AnimatedElementList.emplace("Camera", animElement);

		StoryboardDirectorComponent::Data data((*storyboards)[1], std::unordered_map<std::string, eid_t>{ {"Camera", entityId->storyboard}, { "Player", entityId->player }});

#ifndef _DEBUG
		component->BeginStoryboard<YardSceneTurnAroundStoryboardEndEvent>(data, (*storyboards)[0].EndTime);
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

	prefabsSteup = true;
}