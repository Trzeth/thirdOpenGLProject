#include "YardScene.h"

#include <thirdEngine/Renderer/Box.h>

#include "Game/Component/TransformComponent.h"
#include "Game/Component/ModelRenderComponent.h"
#include "Game/Component/CameraComponent.h"
#include "Game/Component/PlayerComponent.h"
#include "Game/Extra/PrefabConstructionInfo.h"

void YardScene::Setup()
{
	setupPrefab();

	PrefabConstructionInfo playerInfo = PrefabConstructionInfo(Transform(glm::vec3(0, 0, 0)));
	eid_t player = world.ConstructPrefab(playerPrefab);

	eid_t camera = world.ConstructPrefab(cameraPrefab, player);

	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(player);
	playerComponent->data.camera = camera;

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
	Transform cameraTransform;
	cameraTransform.SetPosition(glm::vec3(0, 10, 10));
	cameraPrefab.AddConstructor(new TransformComponentConstructor(cameraTransform));

	cameraPrefab.AddConstructor(new CameraComponentConstructor(CameraComponent::Data()));

	prefabsSteup = true;
}