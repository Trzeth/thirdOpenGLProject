#include "ClothStoreScene.h"

#include "Game/Scene/SceneManager.h"

#include "Game/Component/TransformComponent.h"
#include "Game/Component/ModelRenderComponent.h"
#include "Game/Component/CameraComponent.h"
#include "Game/Component/CharacterShowcaseComponent.h"

#include "Game/Event/ClothStoreSceneEvent.h"

#include "Game/GlobalVariable.h"
#include "Game/Scene/Town/TownScene.h"

#include <thirdEngine/Renderer/Box.h>

void ClothStoreScene::Setup()
{
	setupPrefab();

	eid_t player = world.ConstructPrefab(playerPrefab);
	eid_t camera = world.ConstructPrefab(cameraPrefab);
	world.ConstructPrefab(skyboxPrefab);

	CameraComponent* cameraComponent = world.GetComponent<CameraComponent>(camera);
	cameraComponent->isEnable = true;
}

void ClothStoreScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	DirLight dirLight;
	dirLight.direction = glm::vec3(0.0, 0.0, -1);
	dirLight.ambient = glm::vec3(1.0);
	dirLight.diffuse = glm::vec3(0.15);
	dirLight.specular = glm::vec3(0.0);
	renderer.SetDirLight(dirLight);

	interactEventResponder = std::make_shared<InteractEventResponder>(world, eventManager);

	lightingShader = shaderLoader.BuildFromFile("Shaders/lightingShader.vert", "Shaders/lightingShader.frag");

	/* Player */
	{
		glm::mat4 playerModelMat4(1.0f);
		playerModelMat4 *= glm::scale(playerModelMat4, glm::vec3(0.5f));

		playerWalkModel = modelLoader.LoadFromFile("Resources/Walk.DAE", ModelLoadingPrefab::Default, playerModelMat4);
		Renderer::ModelHandle playerWalkModelHandle = renderer.GetModelHandle(playerWalkModel);

		Material i = playerWalkModel.GetMeshMaterial(0);
		i.SetProperty("texture_diffuse", MaterialProperty(globalVariable.clothes[globalVariable.clothIndex]));
		playerWalkModel.SetMeshMaterial(0, i);

		playerPrefab.SetName("Character");
		playerPrefab.AddConstructor(new TransformComponentConstructor(glm::vec3(-6, 0, 0)));

		playerPrefab.AddConstructor(new CharacterShowcaseComponentConstructor(CharacterShowcaseComponent::Data()));
		playerPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, playerWalkModelHandle, lightingShader));
	}

	/* Skybox */
	{
		Model skyboxModel = Box::GetSkybox("Resources/back/", std::vector<std::string>{"0.png", "0.png", "0.png", "0.png", "0.png", "0.png"});
		skyboxShader = shaderLoader.BuildFromFile("Shaders/skybox.vert", "Shaders/skybox.frag");

		Renderer::ModelHandle skyboxModelHandle = renderer.GetModelHandle(skyboxModel);

		skyboxPrefab.AddConstructor(new TransformComponentConstructor());
		skyboxPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, skyboxModelHandle, skyboxShader));
	}

	/* Camera */
	{
		cameraPrefab.AddConstructor(new TransformComponentConstructor(glm::vec3(0, 4, 15)));
		cameraPrefab.AddConstructor(new CameraComponentConstructor(CameraComponent::Data()));
	}

	/* UI */
	{
		clothStoreUI = std::make_shared<ClothStoreUI>(eventManager);
		clothStoreUIHandle = uiRenderer.GetEntityHandle(clothStoreUI);
	}

	/* Callback */
	{
		std::function<void(const ClothStoreChangeClothSceneEvent& event)> doorInteractCallback =
			[scene = this, &g = globalVariable, &playerWalkModel = playerWalkModel, &sceneManager = sceneManager](const ClothStoreChangeClothSceneEvent& event) {
			if (event.clothIndex == 4)
			{
				sceneManager.SetSpawnPosition(glm::vec3(-14.22, 0, -78.43));
				LoadingScreenInfo info;
				info.LoopTime = 1.0f;
				info.ImageWidth = 400;
				info.ImageHeight = 400;
				info.LoadingImagePath = std::vector<std::string>{ "GUI/Loading/Key/0.png","GUI/Loading/Key/1.png","GUI/Loading/Key/2.png" };

				sceneManager.LoadScene<TownScene>(info);
				return;
			}
			g.clothIndex = event.clothIndex;

			Material i = playerWalkModel.GetMeshMaterial(0);
			i.SetProperty("texture_diffuse", MaterialProperty(g.clothes[g.clothIndex]));
			playerWalkModel.SetMeshMaterial(0, i);
		};

		eventManager.RegisterForEvent<ClothStoreChangeClothSceneEvent>(doorInteractCallback);
	}

	prefabsSteup = true;
}

void ClothStoreScene::Finish()
{
	renderer.GenVAO();
}

void ClothStoreScene::PreDestruct()
{
	eventManager.ClearEventListener<ClothStoreChangeClothSceneEvent>();
}