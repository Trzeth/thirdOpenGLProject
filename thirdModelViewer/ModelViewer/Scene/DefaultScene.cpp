#include "DefaultScene.h"

#include "ModelViewer/Component/TransformComponent.h"
#include "ModelViewer/Component/CameraComponent.h"
#include "ModelViewer/Component/ModelRenderComponent.h"
#include "ModelViewer/Component/ViewerComponent.h"

#include "ModelViewer/Extra/PrefabConstructionInfo.h"

void DefaultScene::Setup()
{
	setupPrefab();

	eid_t model = world.ConstructPrefab(modelPrefab, World::NullEntity);

	cameraPrefab.SetName("FPSCamera");
	eid_t fpsCamera = world.ConstructPrefab(cameraPrefab, World::NullEntity);

	cameraPrefab.SetName("FixedCamera");
	eid_t fixedCamera = world.ConstructPrefab(cameraPrefab, World::NullEntity);

	ViewerComponent* viewerComponent = world.GetComponent<ViewerComponent>(model);
	viewerComponent->data.FPSCamera = fpsCamera;
	viewerComponent->data.FixedCamera = fixedCamera;
	viewerComponent->viewerState = ViewerState::FPSCamera;
}

void DefaultScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	shader = shaderLoader.CompileAndLink("ModelViewer/Shader/NoLight/shader.vs", "ModelViewer/Shader/NoLight/shader.fs");
	Model model = modelLoader.LoadModel("sphere.obj");
	model.GenVAO();

	Renderer::ModelHandle modelHandle = renderer.GetModelHandle(model);

	modelPrefab.SetName("Object Model");
	modelPrefab.AddConstructor(new TransformComponentConstructor());
	modelPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, modelHandle, shader));
	modelPrefab.AddConstructor(new ViewerComponentConstructor(ViewerComponent::Data()));

	cameraPrefab.SetName("Camera");
	cameraPrefab.AddConstructor(new TransformComponentConstructor(Transform(glm::vec3(0, 0, 10))));
	cameraPrefab.AddConstructor(new CameraComponentConstructor(Camera()));

	prefabsSteup = true;
}