#include "ForestScene.h"

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

#include "Game/Event/HouseSceneEvent.h"

#include "Game/Scene/Home/YardScene.h"
#include <thirdEngine/Renderer/Box.h>

void ForestScene::Setup()
{
	setupPrefab();

	eid_t player = world.ConstructPrefab(playerPrefab, World::NullEntity, new PrefabConstructionInfo(Transform(glm::vec3(-47, 0, -222))));
	eid_t camera = world.ConstructPrefab(cameraPrefab);

	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(player);
	playerComponent->data.camera = camera;
	playerComponent->SetControlState(PlayerControlState::Normal);

	CameraComponent* cameraComponent = world.GetComponent<CameraComponent>(camera);
	cameraComponent->isEnable = true;

	world.ConstructPrefab(forestPrefab);
	world.ConstructPrefab(doorInteractPrefab);
	world.ConstructPrefab(skyboxPrefab);
}

void ForestScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	/* Forest */
	{
		glm::mat4 forestModelMat4(1.0f);
		forestModelMat4 *= glm::translate(forestModelMat4, glm::vec3(0, -1.0, 0));
		forestModelMat4 *= glm::scale(forestModelMat4, glm::vec3(0.1f));

		DirLight dirLight;
		dirLight.direction = glm::vec3(0.02, -9.37, 0.08);
		dirLight.ambient = glm::vec3(0.61, 0.6, 0.5);
		dirLight.diffuse = glm::vec3(0.7, 0.7, 0.75);
		dirLight.specular = glm::vec3(0.5, 0.5, 0.5);
		renderer.SetDirLight(dirLight);

		Model forestModel = modelLoader.LoadFromFile("Resources/CG_Forest/Forest.FBX", ModelLoadingPrefab::Optimize, forestModelMat4, true);
		Renderer::ModelHandle forestModelHandle = renderer.GetModelHandle(forestModel);

		forestPrefab.SetName("ForestModel");

		lightingShader = shaderLoader.BuildFromFile("Shaders/lightingShader.vert", "Shaders/lightingShader.frag");

		forestPrefab.AddConstructor(new TransformComponentConstructor());
		forestPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, forestModelHandle, lightingShader, true));

		b2BodyDef bodyDef;

		std::vector<b2FixtureDef> fixtures;

		b2FixtureDef left;
		b2PolygonShape* s1 = new b2PolygonShape();
		s1->SetAsBox(0.1f, 13.5f, b2Vec2(-17.5f, -6.3f), 0);
		left.shape = s1;

		b2FixtureDef back;
		b2PolygonShape* s2 = new b2PolygonShape();
		s2->SetAsBox(13.0f, 5.0f, b2Vec2(-4.0f, -20.0f), 0);
		back.shape = s2;

		b2FixtureDef right;
		b2PolygonShape* s3 = new b2PolygonShape();
		s3->SetAsBox(0.1f, 13.5f, b2Vec2(9.6f, -6.3f), 0);
		right.shape = s3;

		b2FixtureDef front;
		b2PolygonShape* s4 = new b2PolygonShape();
		s4->SetAsBox(13.0f, 0.1f, b2Vec2(-4.0f, 7.5f), 0);
		front.shape = s4;

		fixtures.push_back(left);
		fixtures.push_back(back);
		fixtures.push_back(right);
		fixtures.push_back(front);

		//forestPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, fixtures)));
	}

	/* Skybox */
	{
		Model skyboxModel = Box::GetSkybox("Resources/skybox/", std::vector<std::string>{"right.jpg", "left.jpg", "up.jpg", "down.jpg", "front.jpg", "back.jpg"});
		skyboxShader = shaderLoader.BuildFromFile("Shaders/skybox.vert", "Shaders/skybox.frag");

		Renderer::ModelHandle skyboxModelHandle = renderer.GetModelHandle(skyboxModel);

		skyboxPrefab.AddConstructor(new TransformComponentConstructor());
		skyboxPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, skyboxModelHandle, skyboxShader));
	}

	interactEventResponder = std::make_shared<InteractEventResponder>(world, eventManager);

	/* Scene Interact Object */
	{
		/* Door */
		b2BodyDef doorDef;

		b2FixtureDef door;
		door.isSensor = true;
		b2PolygonShape* s1 = new b2PolygonShape();
		s1->SetAsBox(5.0f, 5.0f, b2Vec2(-4.0f, -19.0f), 0);
		door.shape = s1;

		doorInteractPrefab.SetName("Door Interact");
		doorInteractPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(doorDef, door)));
		doorInteractPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(HouseSceneDoorInteractEvent).hash_code())));
	}

	/* Player */
	{
		glm::mat4 playerModelMat4(1.0f);
		playerModelMat4 *= glm::scale(playerModelMat4, glm::vec3(0.5f));

		Model playerWalkModel = modelLoader.LoadFromFile("Resources/Walk.DAE", ModelLoadingPrefab::Default, playerModelMat4);
		Model playerPickLetterModel = modelLoader.LoadFromFile("Resources/PickLetter.DAE", ModelLoadingPrefab::Default, playerModelMat4);
		Model playerPutLetterModel = modelLoader.LoadFromFile("Resources/PutLetter.DAE", ModelLoadingPrefab::Default, playerModelMat4);
		Model playerTurnAroundModel = modelLoader.LoadFromFile("Resources/TurnAround.DAE", ModelLoadingPrefab::Default, playerModelMat4);

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
		std::function<void(const HouseSceneDoorInteractEvent& event)> doorInteractCallback =
			[scene = this, &sceneManager = sceneManager](const HouseSceneDoorInteractEvent& event) {
			LoadingScreenInfo info;
			info.LoopTime = 1.0f;
			info.LoadingImagePath = std::vector<std::string>{ "GUI/Loading1.png" };

			sceneManager.LoadScene<YardScene>(info);
		};

		eventManager.RegisterForEvent<HouseSceneDoorInteractEvent>(doorInteractCallback);
	}

	prefabsSteup = true;
}

void ForestScene::Finish()
{
	glEnable(GL_CULL_FACE);
	renderer.GenVAO();
}

void ForestScene::PreDestruct()
{
	glDisable(GL_CULL_FACE);
}