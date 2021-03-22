#include "DefaultScene.h"

#include "ModelViewer/Component/TransformComponent.h"
#include "ModelViewer/Component/CameraComponent.h"
#include "ModelViewer/Component/ModelRenderComponent.h"
#include "ModelViewer/Component/PlayerComponent.h"

#include "ModelViewer/Extra/PrefabConstructionInfo.h"

void DefaultScene::Setup()
{
	setupPrefab();

	PrefabConstructionInfo playerInfo = PrefabConstructionInfo(Transform(glm::vec3(0, 0, 0)));
	eid_t man = world.ConstructPrefab(manPrefab, World::NullEntity, &playerInfo);

	eid_t camera = world.ConstructPrefab(cameraPrefab, man);

	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(man);
	playerComponent->data.camera = camera;

	world.ConstructPrefab(scenePrefab);
}

void DefaultScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	shader = shaderLoader.CompileAndLink("ModelViewer/Shader/shaderWithShadow.vs", "ModelViewer/Shader/shaderWithShadow.fs");
	Model man = modelLoader.LoadModel("Walk.DAE", ModelType::Normal);
	Renderer::ModelHandle manModelHandle = renderer.GetModelHandle(man);
	PlayerComponent::Data playerData;

	manPrefab.SetName("Player");
	manPrefab.AddConstructor(new TransformComponentConstructor());
	manPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, manModelHandle, shader));
	manPrefab.AddConstructor(new PlayerComponentConstructor(playerData));

	//Model scene = modelLoader.LoadModel("../Shared/Resources/Scene/Scene_1_3.FBX", ModelType::Normal);
	//Renderer::ModelHandle sceneModelHandle = renderer.GetModelHandle(scene);
	//scenePrefab.SetName("Scene");
	//scenePrefab.AddConstructor(new TransformComponentConstructor());
	//scenePrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, sceneModelHandle, shader));

	cameraPrefab.SetName("Camera");
	cameraPrefab.AddConstructor(new TransformComponentConstructor(Transform(glm::vec3(0, 10, 10))));
	cameraPrefab.AddConstructor(new CameraComponentConstructor(Camera()));

	prefabsSteup = true;
}