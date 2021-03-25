#include "DefaultScene.h"

#include "ModelViewer/Component/TransformComponent.h"
#include "ModelViewer/Component/CameraComponent.h"
#include "ModelViewer/Component/ModelRenderComponent.h"
#include "ModelViewer/Component/ObjectViewerComponent.h"
#include "ModelViewer/Component/InspectorComponent.h"

#include "ModelViewer/Extra/PrefabConstructionInfo.h"

void DefaultScene::Setup()
{
	setupPrefab();

	eid_t model = world.ConstructPrefab(modelPrefab, World::NullEntity);

	cameraPrefab.SetName("FPSCamera");
	eid_t fpsCamera = world.ConstructPrefab(cameraPrefab, World::NullEntity);

	cameraPrefab.SetName("FixedCamera");
	eid_t fixedCamera = world.ConstructPrefab(cameraPrefab, World::NullEntity);

	ObjectViewerComponent* viewerComponent = world.GetComponent<ObjectViewerComponent>(model);
	viewerComponent->data.FPSCamera = fpsCamera;
	viewerComponent->data.FixedCamera = fixedCamera;
	viewerComponent->viewerState = ViewerState::FPSCamera;

	world.ConstructPrefab(inspectorPrefab, World::NullEntity);
}

void DefaultScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	shader = shaderLoader.CompileAndLink("ModelViewer/Shader/NoLight/shader.vs", "ModelViewer/Shader/NoLight/shader.fs");
	Model model = modelLoader.LoadModel("Walk.DAE");
	model.GenVAO();

	Renderer::ModelHandle modelHandle = renderer.GetModelHandle(model);

	modelPrefab.SetName("Object Model");
	modelPrefab.AddConstructor(new TransformComponentConstructor());
	modelPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, modelHandle, shader, "combinedAnim"));
	modelPrefab.AddConstructor(new ObjectViewerComponentConstructor(ObjectViewerComponent::Data()));

	cameraPrefab.SetName("Camera");
	cameraPrefab.AddConstructor(new TransformComponentConstructor(Transform(glm::vec3(0, 0, 0))));
	cameraPrefab.AddConstructor(new CameraComponentConstructor(Camera()));

	inspectorPrefab.SetName("Inspector");
	inspectorPrefab.AddConstructor(new InspectorComponentConstructor(uiRenderer));

	prefabsSteup = true;
}