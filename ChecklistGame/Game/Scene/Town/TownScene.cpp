#include "TownScene.h"

#include <thirdEngine/Renderer/Box.h>
#include "Game/Scene/SceneManager.h"

#include "Game/Component/TransformComponent.h"
#include "Game/Component/ModelRenderComponent.h"
#include "Game/Component/CameraComponent.h"
#include "Game/Component/PlayerComponent.h"
#include "Game/Component/StoryboardDirectorComponent.h"
#include "Game/Component/CollisionComponent.h"
#include "Game/Component/RigidbodyMotorComponent.h"
#include "Game/Component/InteractComponent.h"

#include "Game/Extra/PrefabConstructionInfo.h"
#include "Game/Event/TownSceneEvent.h"

#include "Game/GlobalVariable.h"

#include "Game/Scene/Town/ShopScene.h"
#include "Game/Scene/Town/ClothStoreScene.h"

void TownScene::Setup()
{
	setupPrefab();

	eid_t player = world.ConstructPrefab(playerPrefab, World::NullEntity, new PrefabConstructionInfo(spawnPosition));
	eid_t camera = world.ConstructPrefab(cameraPrefab);

	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(player);
	playerComponent->data.camera = camera;
	playerComponent->SetControlState(PlayerControlState::Normal);

	CameraComponent* cameraComponent = world.GetComponent<CameraComponent>(camera);
	cameraComponent->isEnable = true;

	world.ConstructPrefab(townPrefab);
	world.ConstructPrefab(shopDoorInteractPrefab);
	world.ConstructPrefab(clothStoreDoorInteractPrefab);
	world.ConstructPrefab(skyboxPrefab);
}

void TownScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	/* Town */
	{
		glm::mat4 townModelMat4(1.0f);
		townModelMat4 *= glm::translate(townModelMat4, glm::vec3(0, -0.2, 0));
		townModelMat4 *= glm::scale(townModelMat4, glm::vec3(20.0f));

		Model townModel = modelLoader.LoadFromFile("Resources/CG_Town/untitled.obj", ModelLoadingPrefab::Optimize, townModelMat4, true);
		Renderer::ModelHandle townModelHandle = renderer.GetModelHandle(townModel);

		DirLight dirLight;
		dirLight.direction = glm::vec3(-1, -1, 1);
		dirLight.ambient = glm::vec3(0.5, 0.5, 0.4);
		dirLight.diffuse = glm::vec3(0.7, 0.7, 0.75);
		dirLight.specular = glm::vec3(0.5, 0.5, 0.5);
		renderer.SetDirLight(dirLight);

		townPrefab.SetName("TownModel");

		lightingShader = shaderLoader.BuildFromFile("Shaders/lightingShader.vert", "Shaders/lightingShader.frag");

		townPrefab.AddConstructor(new TransformComponentConstructor());
		townPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, townModelHandle, lightingShader));
	}

	interactEventResponder = std::make_shared<InteractEventResponder>(world, eventManager);

	/* Scene Interact Object */
	{
		/* Door */
		b2BodyDef bodyDef;

		b2FixtureDef clothStoreDoor;
		clothStoreDoor.isSensor = true;
		b2PolygonShape* s2 = new b2PolygonShape();
		s2->SetAsBox(1.58f, 1.12f, b2Vec2(-20.71f, -76.97f), 0);
		clothStoreDoor.shape = s2;

		clothStoreDoorInteractPrefab.SetName("Cloth Store Door Interact");
		clothStoreDoorInteractPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, clothStoreDoor)));
		clothStoreDoorInteractPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(TownSceneClothStoreDoorInteractEvent).hash_code())));

		b2FixtureDef shopDoor;
		shopDoor.isSensor = true;
		b2PolygonShape* s1 = new b2PolygonShape();
		s1->SetAsBox(5.5f, 1.58f, b2Vec2(25.18f, 39.50f), 0);
		shopDoor.shape = s1;

		shopDoorInteractPrefab.SetName("Shop Door Interact");
		shopDoorInteractPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, shopDoor)));
		shopDoorInteractPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(TownSceneShopDoorInteractEvent).hash_code())));
	}

	/* Skybox */
	{
		Model skyboxModel = Box::GetSkybox("Resources/skybox/", std::vector<std::string>{"right.jpg", "left.jpg", "up.jpg", "down.jpg", "front.jpg", "back.jpg"});
		skyboxShader = shaderLoader.BuildFromFile("Shaders/skybox.vert", "Shaders/skybox.frag");

		Renderer::ModelHandle skyboxModelHandle = renderer.GetModelHandle(skyboxModel);

		skyboxPrefab.AddConstructor(new TransformComponentConstructor());
		skyboxPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, skyboxModelHandle, skyboxShader));
	}

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
		playerPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, playerData.walk, lightingShader, true));
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

	/* Callback */
	{
		std::function<void(const TownSceneShopDoorInteractEvent& event)> shopDoorInteractCallback =
			[scene = this, &sceneManager = sceneManager](const TownSceneShopDoorInteractEvent& event) {
			LoadingScreenInfo info;
			info.LoopTime = 1.0f;
			info.ImageWidth = 400;
			info.ImageHeight = 400;
			info.LoadingImagePath = std::vector<std::string>{ "GUI/Loading/Key/0.png","GUI/Loading/Key/1.png","GUI/Loading/Key/2.png" };

			sceneManager.LoadScene<ShopScene>(info);
		};

		eventManager.RegisterForEvent<TownSceneShopDoorInteractEvent>(shopDoorInteractCallback);

		std::function<void(const TownSceneClothStoreDoorInteractEvent& event)> clothStoreDoorInteractCallback =
			[scene = this, &sceneManager = sceneManager](const TownSceneClothStoreDoorInteractEvent& event) {
			LoadingScreenInfo info;
			info.LoopTime = 1.0f;
			info.ImageWidth = 400;
			info.ImageHeight = 400;
			info.LoadingImagePath = std::vector<std::string>{ "GUI/Loading/Key/0.png","GUI/Loading/Key/1.png","GUI/Loading/Key/2.png" };

			sceneManager.LoadScene<ClothStoreScene>(info);
		};

		eventManager.RegisterForEvent<TownSceneClothStoreDoorInteractEvent>(clothStoreDoorInteractCallback);
	}

	prefabsSteup = true;
}

void TownScene::Finish()
{
	renderer.GenVAO();
}

void TownScene::PreDestruct()
{
	eventManager.ClearEventListener<TownSceneClothStoreDoorInteractEvent>();
	eventManager.ClearEventListener<TownSceneShopDoorInteractEvent>();
}