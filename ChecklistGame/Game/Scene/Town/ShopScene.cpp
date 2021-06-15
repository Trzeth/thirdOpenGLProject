#include "ShopScene.h"

#include "Game/Scene/SceneManager.h"

#include "Game/Component/TransformComponent.h"
#include "Game/Component/ModelRenderComponent.h"
#include "Game/Component/CameraComponent.h"
#include "Game/Component/PlayerComponent.h"
#include "Game/Component/StoryboardDirectorComponent.h"
#include "Game/Component/CollisionComponent.h"
#include "Game/Component/RigidbodyMotorComponent.h"
#include "Game/Component/InteractComponent.h"

#include "Game/Event/ShopSceneEvent.h"
#include "Game/GlobalVariable.h"
#include "Game/Scene/Town/TownScene.h"

void ShopScene::Setup()
{
	setupPrefab();

	eid_t player = world.ConstructPrefab(playerPrefab);
	eid_t camera = world.ConstructPrefab(cameraPrefab);

	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(player);
	playerComponent->data.camera = camera;
	playerComponent->SetControlState(PlayerControlState::Normal);

	CameraComponent* cameraComponent = world.GetComponent<CameraComponent>(camera);
	cameraComponent->isEnable = true;

	world.ConstructPrefab(shopPrefab);
	world.ConstructPrefab(doorInteractPrefab);
}

void ShopScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	/* House */
	{
		glm::mat4 shopModelMat4(1.0f);
		shopModelMat4 *= glm::rotate(shopModelMat4, glm::radians(90.0f), glm::vec3(0, 1, 0));
		shopModelMat4 *= glm::scale(shopModelMat4, glm::vec3(400.0f));

		Model shopModel = modelLoader.LoadFromFile("Resources/CG_SHOP/untitled.obj", ModelLoadingPrefab::Optimize, shopModelMat4, true);
		Renderer::ModelHandle shopModelHandle = renderer.GetModelHandle(shopModel);

		DirLight dirLight;
		dirLight.direction = glm::vec3(0.04, -1, -0.01);
		dirLight.ambient = glm::vec3(0.8, 0.8, 0.8);
		dirLight.diffuse = glm::vec3(0.5, 0.5, 0.5);
		dirLight.specular = glm::vec3(0.5, 0.5, 0.5);
		renderer.SetDirLight(dirLight);

		shopPrefab.SetName("ShopModel");

		lightingShader = shaderLoader.BuildFromFile("Shaders/lightingShader.vert", "Shaders/lightingShader.frag");

		shopPrefab.AddConstructor(new TransformComponentConstructor());
		shopPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, shopModelHandle, lightingShader));
	}

	interactEventResponder = std::make_shared<InteractEventResponder>(world, eventManager);

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

		playerPrefab.SetName("Player");
		playerPrefab.AddConstructor(new TransformComponentConstructor());

		PlayerComponent::Data playerData;
		playerData.walk = renderer.GetModelHandle(playerWalkModel);
		playerData.pickLetter = renderer.GetModelHandle(playerPickLetterModel);
		playerData.putLetter = renderer.GetModelHandle(playerPutLetterModel);
		playerData.turnAround = renderer.GetModelHandle(playerTurnAroundModel);

		playerPrefab.AddConstructor(new PlayerComponentConstructor(playerData));
		playerPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, playerData.walk, lightingShader));
		b2BodyDef bodyDef;
		bodyDef.type = b2BodyType::b2_dynamicBody;
		bodyDef.allowSleep = false;

		b2FixtureDef fixtureDef;
		b2CircleShape* shape = new b2CircleShape();
		shape->m_radius = 2.0f;
		fixtureDef.shape = shape;
		fixtureDef.density = 0.1f;
		fixtureDef.friction = 0.3f;

		playerPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, std::vector<b2FixtureDef>{fixtureDef})));
		playerPrefab.AddConstructor(new RigidbodyMotorComponentConstructor(RigidbodyMotorComponent::Data()));
	}

	/* Camera */
	{
		cameraPrefab.AddConstructor(new TransformComponentConstructor());
		cameraPrefab.AddConstructor(new CameraComponentConstructor(CameraComponent::Data()));
	}

	/* UI */
	{
		shopUI = std::make_shared<ShopUI>(eventManager);
		shopUIHandle = uiRenderer.GetEntityHandle(shopUI);
	}

	/* Callback */
	{
		std::function<void(const ShopSceneFinishEvent& event)> finishCallback =
			[scene = this, &sceneManager = sceneManager](const ShopSceneFinishEvent& event) {
			sceneManager.SetSpawnPosition(glm::vec3(24.71, 0, 31.76));

			LoadingScreenInfo info;
			info.LoopTime = 1.0f;
			info.ImageWidth = 400;
			info.ImageHeight = 400;
			info.LoadingImagePath = std::vector<std::string>{ "GUI/Loading/Key/0.png","GUI/Loading/Key/1.png","GUI/Loading/Key/2.png" };

			sceneManager.LoadScene<TownScene>(info);
		};

		eventManager.RegisterForEvent<ShopSceneFinishEvent>(finishCallback);
	}

	prefabsSteup = true;
}

void ShopScene::Finish()
{
	glEnable(GL_CULL_FACE);
	renderer.GenVAO();
}

void ShopScene::PreDestruct()
{
	glDisable(GL_CULL_FACE);
	eventManager.ClearEventListener<ShopSceneFinishEvent>();
}