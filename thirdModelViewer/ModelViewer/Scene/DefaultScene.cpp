#include "DefaultScene.h"

#include <thirdEngine/Renderer/Box.h>

#include "ModelViewer/Component/TransformComponent.h"
#include "ModelViewer/Component/CameraComponent.h"
#include "ModelViewer/Component/ModelRenderComponent.h"
#include "ModelViewer/Component/ObjectViewerComponent.h"
#include "ModelViewer/Component/InspectorComponent.h"

#include "ModelViewer/Material/MaterialWrapper.h"
#include "ModelViewer/Material/PBRMaterial.h"
#include "ModelViewer/Material/TexturePBRMaterial.h"
#include "ModelViewer/Material/MaterialLoader.h"

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
	viewerComponent->FPSCamera = fpsCamera;
	viewerComponent->FixedCamera = fixedCamera;
	viewerComponent->viewerState = ViewerState::FPSCamera;

	world.ConstructPrefab(inspectorPrefab, World::NullEntity, &viewer);
}

void DefaultScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	shader = shaderLoader.BuildFromFile("ModelViewer/Shader/NoLight/shader.vs", "ModelViewer/Shader/NoLight/shader.fs");
	Model model = modelLoader.LoadFromFile(u8"sphere.obj");
	model.GenVAO();

	Renderer::ModelHandle modelHandle = renderer.GetModelHandle(model);

	skyboxShader = shaderLoader.BuildFromFile("ModelViewer/Shader/skybox.vs", "ModelViewer/Shader/skybox.fs");
	HDRSkyboxReturn skyboxReturn = Box::GetHDRSkybox("photo_studio_01_1k.hdr");
	Model skyboxModel = skyboxReturn.HDRSkybox;
	skyboxModel.GenVAO();

	skybox = renderer.GetRenderableHandle(renderer.GetModelHandle(skyboxModel), skyboxShader);

	viewerPrefab.SetName("Viewer");
	viewerPrefab.AddConstructor(new TransformComponentConstructor());
	viewerPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, modelHandle, shader));

	std::vector<MaterialWrapper> materialList = getMaterialList();

	MaterialLoader materialLoader;

	materialList.push_back(
		MaterialWrapper("test", ShaderFileInfo("ModelViewer/Shader/pbr.vs", "ModelViewer/Shader/texturePBR.fs"), materialLoader.LoadFromFile("4-steps-stairs-ue/4-steps-stairs.obj.thirdmtl"))
	);

	for (int i : std::vector<int>{ 0,1,4 }) {
		materialList[i].material.SetProperty("irradianceMap", MaterialProperty(skyboxReturn.Irradiancemap));
		materialList[i].material.SetProperty("prefilterMap", MaterialProperty(skyboxReturn.Prefiltermap));
		materialList[i].material.SetProperty("brdfLUT", MaterialProperty(skyboxReturn.BRDFLUT));
	}

	viewerPrefab.AddConstructor(new ObjectViewerComponentConstructor(materialList));

	cameraPrefab.SetName("Camera");
	cameraPrefab.AddConstructor(new TransformComponentConstructor(Transform(glm::vec3(0, 0, 0))));
	cameraPrefab.AddConstructor(new CameraComponentConstructor(Camera()));

	inspectorPrefab.SetName("Inspector");
	inspectorPrefab.AddConstructor(new InspectorComponentConstructor(uiRenderer));

	prefabsSteup = true;
}

std::vector<MaterialWrapper> DefaultScene::getMaterialList()
{
	std::vector<MaterialWrapper> materialList{
		MaterialWrapper("PBR",ShaderFileInfo("ModelViewer/Shader/pbr.vs","ModelViewer/Shader/pbr.fs"),PBRMaterial()),
		MaterialWrapper("TexturePBR",ShaderFileInfo("ModelViewer/Shader/pbr.vs","ModelViewer/Shader/texturePBR.fs"),TexturePBRMaterial()),
		MaterialWrapper("Plain",ShaderFileInfo("ModelViewer/Shader/NoLight/shader.vs", "ModelViewer/Shader/NoLight/shader.fs"),Material()),
		MaterialWrapper("Skinned",ShaderFileInfo("ModelViewer/Shader/NoLight/skinnedShader.vs", "ModelViewer/Shader/NoLight/skinnedShader.fs"),Material())
	};

	return materialList;
}