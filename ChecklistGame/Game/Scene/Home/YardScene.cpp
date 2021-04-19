#include "YardScene.h"

#include <thirdEngine/Renderer/Box.h>

#include "Game/Component/TransformComponent.h"
#include "Game/Component/ModelRenderComponent.h"
#include "Game/Component/CameraComponent.h"
#include "Game/Component/PlayerComponent.h"
#include "Game/Component/StoryboardDirectorComponent.h"

#include "Game/Event/StartGameEvent.h"

#include "Game/Extra/PrefabConstructionInfo.h"

void YardScene::Setup()
{
	setupPrefab();

	PrefabConstructionInfo playerInfo = PrefabConstructionInfo(Transform(glm::vec3(0, 0, 0)));
	entityId.player = world.ConstructPrefab(playerPrefab);

	entityId.camera = world.ConstructPrefab(cameraPrefab, entityId.player);

	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(entityId.player);
	playerComponent->data.camera = entityId.camera;
	playerComponent->playerState = PlayerState::PlayerState_InGUI;

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

	Model playerModel = modelLoader.LoadModel("Resources/Walk.DAE", playerModelMat4);
	playerModel.GenVAO();

	playerPrefab.SetName("Player");
	Renderer::ModelHandle playerModelHandle = renderer.GetModelHandle(playerModel);
	playerPrefab.AddConstructor(new TransformComponentConstructor());
	playerPrefab.AddConstructor(new PlayerComponentConstructor(PlayerComponent::Data()));
	playerPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, playerModelHandle, skinnedShader));

	/* Camera */
	cameraPrefab.AddConstructor(new TransformComponentConstructor());
	cameraPrefab.AddConstructor(new CameraComponentConstructor(CameraComponent::Data()));

	/* Storyboard */
	storyboardPrefab.AddConstructor(new StoryboardDirectorComponentConstructor(StoryboardDirectorComponent::Data()));
	storyboardPrefab.AddConstructor(new CameraComponentConstructor(CameraComponent::Data()));
	Transform storyboardCameraTransform;
	storyboardCameraTransform.SetPosition(glm::vec3(-4, 50, 30));
	storyboardCameraTransform.SetRotation(glm::angleAxis(glm::radians(5.0f), glm::vec3(1, 0, 0)));
	storyboardPrefab.AddConstructor(new TransformComponentConstructor(storyboardCameraTransform));

	storyboards = storyboardLoader.LoadFromFile("Storyboards/Yard.thirdsbs");

	/* UI */
	startMenu = std::make_shared<StartMenu>(eventManager);
	startMenuHandle = uiRenderer.GetEntityHandle(startMenu);

	std::function<void(const StartGameEvent& event)> startGameCallback =
		[scene = this, entityId = &entityId, world = &world, storyboards = &storyboards](const StartGameEvent& event) {
		StoryboardDirectorComponent* component = world->GetComponent<StoryboardDirectorComponent>(entityId->storyboard);
		component->data.currentStoryboard = (*storyboards)[0];
		component->data.storyboardState = StoryboardState::BeginFlag;
		component->data.nameIdMap.clear();
		component->data.nameIdMap.emplace("Camera", entityId->storyboard);
		component->data.nameIdMap.emplace("Player", entityId->player);
		component->currentTime = 0;
	};
	eventManager.RegisterForEvent<StartGameEvent>(startGameCallback);

	prefabsSteup = true;
}