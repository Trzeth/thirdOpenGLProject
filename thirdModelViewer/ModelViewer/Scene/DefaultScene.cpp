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

	eid_t viewer = world.ConstructPrefab(viewerPrefab, World::NullEntity);

	cameraPrefab.SetName("FPSCamera");
	eid_t fpsCamera = world.ConstructPrefab(cameraPrefab, World::NullEntity);

	cameraPrefab.SetName("FixedCamera");
	eid_t fixedCamera = world.ConstructPrefab(cameraPrefab, World::NullEntity);

	ObjectViewerComponent* viewerComponent = world.GetComponent<ObjectViewerComponent>(viewer);
	viewerComponent->data.FPSCamera = fpsCamera;
	viewerComponent->data.FixedCamera = fixedCamera;
	viewerComponent->viewerState = ViewerState::FPSCamera;

	InspectorComponentConstructorInfo info;
	info.viewer = viewer;
	info.shaderFileInfos = std::vector<ShaderFileInfo>{
		ShaderFileInfo("Plain","ModelViewer/Shader/NoLight/shader.vs", "ModelViewer/Shader/NoLight/shader.fs"),
		ShaderFileInfo("Skinned","ModelViewer/Shader/NoLight/skinnedShader.vs", "ModelViewer/Shader/NoLight/skinnedShader.fs"),
	};

	eid_t inspector = world.ConstructPrefab(inspectorPrefab, World::NullEntity, &info);
}

void DefaultScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	shader = shaderLoader.CompileAndLink("ModelViewer/Shader/NoLight/shader.vs", "ModelViewer/Shader/NoLight/shader.fs");
	Model model = modelLoader.LoadModel(u8"sphere.obj");
	model.GenVAO();

	Renderer::ModelHandle modelHandle = renderer.GetModelHandle(model);

	viewerPrefab.SetName("Viewer");
	viewerPrefab.AddConstructor(new TransformComponentConstructor());
	viewerPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, modelHandle, shader));
	viewerPrefab.AddConstructor(new ObjectViewerComponentConstructor(ObjectViewerComponent::Data()));

	cameraPrefab.SetName("Camera");
	cameraPrefab.AddConstructor(new TransformComponentConstructor(Transform(glm::vec3(0, 0, 0))));
	cameraPrefab.AddConstructor(new CameraComponentConstructor(Camera()));

	inspectorPrefab.SetName("Inspector");
	inspectorPrefab.AddConstructor(new InspectorComponentConstructor(uiRenderer, shaderLoader));

	prefabsSteup = true;
}