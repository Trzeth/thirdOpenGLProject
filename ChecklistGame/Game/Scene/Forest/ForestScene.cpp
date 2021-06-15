#include "ForestScene.h"

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

#include "Game/GlobalVariable.h"

#include "Game/Extra/PrefabConstructionInfo.h"

#include "Game/Scene/Home/YardScene.h"
#include "Game/Event/ForestSceneEvent.h"

void ForestScene::Setup()
{
	setupPrefab();

	eid_t player = world.ConstructPrefab(playerPrefab, World::NullEntity, new PrefabConstructionInfo(Transform(glm::vec3(-47.66, 0, -176.19))));
	eid_t camera = world.ConstructPrefab(cameraPrefab);

	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(player);
	playerComponent->data.camera = camera;
	playerComponent->SetControlState(PlayerControlState::Normal);

	CameraComponent* cameraComponent = world.GetComponent<CameraComponent>(camera);
	cameraComponent->isEnable = true;

	world.ConstructPrefab(forestPrefab);

	world.ConstructPrefab(npcInteractPrefab);
	world.ConstructPrefab(applePrefab, World::NullEntity, new PrefabConstructionInfo(Transform(glm::vec3(35.55, -17.0, -71.08))));
	world.ConstructPrefab(mushroomPrefab, World::NullEntity, new PrefabConstructionInfo(Transform(glm::vec3(-89.55, -18.0, -65.58))));
	world.ConstructPrefab(woodPrefab, World::NullEntity, new PrefabConstructionInfo(Transform(glm::vec3(-21.2798, -16.0, -38.70))));

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
		dirLight.position = glm::vec3(-50, 100, -100);
		dirLight.ambient = glm::vec3(0.61, 0.6, 0.5);
		dirLight.diffuse = glm::vec3(0.7, 0.7, 0.75);
		dirLight.specular = glm::vec3(0.5, 0.5, 0.5);
		renderer.SetDirLight(dirLight);

		Model forestModel = modelLoader.LoadFromFile("Resources/CG_Forest/Forest.FBX", ModelLoadingPrefab::Optimize, forestModelMat4, true);
		Renderer::ModelHandle forestModelHandle = renderer.GetModelHandle(forestModel);

		forestPrefab.SetName("ForestModel");

		lightingShader = shaderLoader.BuildFromFile("Shaders/lightingShader.vert", "Shaders/lightingShader.frag");

		forestPrefab.AddConstructor(new TransformComponentConstructor());
		forestPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, forestModelHandle, lightingShader));
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
		b2BodyDef bodyDef;

		b2FixtureDef npc;
		npc.isSensor = true;
		b2CircleShape* s1 = new b2CircleShape();
		s1->m_p = b2Vec2(-50.23, -121.02);
		s1->m_radius = 2.0f;
		npc.shape = s1;

		npcInteractPrefab.SetName("NPC Interact");
		npcInteractPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, npc)));
		npcInteractPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(ForestSceneNPCInteractEvent).hash_code())));

		b2FixtureDef apple;
		apple.isSensor = true;

		b2CircleShape* s2 = new b2CircleShape();
		s2->m_radius = 2.0f;
		apple.shape = s2;

		glm::mat4 trans(1.0);
		trans = glm::scale(trans, glm::vec3(0.1f));
		//trans = glm::translate(trans, glm::vec3(0, 0, -20.0));

		appleModel = modelLoader.LoadFromFile("Resources/CG_Forest/APPLE.FBX", ModelLoadingPrefab::Optimize, trans, true);
		Renderer::ModelHandle appleModelHandle = renderer.GetModelHandle(appleModel);

		applePrefab.SetName("Apple");
		applePrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, apple)));
		applePrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, appleModelHandle, lightingShader, true));
		applePrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(ForestSceneAppleInteractEvent).hash_code())));
		applePrefab.AddConstructor(new TransformComponentConstructor());

		b2FixtureDef wood;
		wood.isSensor = true;

		b2CircleShape* s3 = new b2CircleShape();
		s3->m_radius = 2.0f;
		wood.shape = s3;

		woodModel = modelLoader.LoadFromFile("Resources/CG_Forest/WOOD.FBX", ModelLoadingPrefab::Optimize, trans, true);
		Renderer::ModelHandle woodModelHandle = renderer.GetModelHandle(woodModel);

		woodPrefab.SetName("Wood");
		woodPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, wood)));
		woodPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, woodModelHandle, lightingShader, true));
		woodPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(ForestSceneWoodInteractEvent).hash_code())));
		woodPrefab.AddConstructor(new TransformComponentConstructor());

		b2FixtureDef mushroom;
		mushroom.isSensor = true;

		b2CircleShape* s4 = new b2CircleShape();
		s4->m_radius = 2.0f;
		mushroom.shape = s4;

		mushroomModel = modelLoader.LoadFromFile("Resources/CG_Forest/MUSHROOM.FBX", ModelLoadingPrefab::Optimize, trans, true);
		Renderer::ModelHandle mushroomModelHandle = renderer.GetModelHandle(mushroomModel);

		mushroomPrefab.SetName("Mushroom");
		mushroomPrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, mushroom)));
		mushroomPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, mushroomModelHandle, lightingShader, true));
		mushroomPrefab.AddConstructor(new InteractComponentConstructor(InteractComponent::Data(typeid(ForestSceneMushroomInteractEvent).hash_code())));
		mushroomPrefab.AddConstructor(new TransformComponentConstructor());
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

	/* UI */

	{
		npcConversation = std::make_shared<DeerNPCConversation>(eventManager);
		npcConversationHandle = uiRenderer.GetEntityHandle(npcConversation);
	}

	/* Callback */
	{
		std::function<void(const ForestSceneNPCInteractEvent& event)> npcInteractCallback =
			[scene = this, &npcConversation = npcConversation, &itemStatus = itemStatus](const ForestSceneNPCInteractEvent& event) {
			if (!itemStatus[3])
			{
				npcConversation->Show();
				itemStatus[3] = true;
				return;
			}
			else if (itemStatus[0] && itemStatus[1] && itemStatus[2])
			{
				npcConversation->Show();
			}
		};

		eventManager.RegisterForEvent<ForestSceneNPCInteractEvent>(npcInteractCallback);

		std::function<void(const ForestSceneGoHomeInteractEvent& event)> gohomeInteractCallback =
			[scene = this, &sceneManager = sceneManager, &globalVariable = globalVariable](const ForestSceneGoHomeInteractEvent& event) {
			LoadingScreenInfo info;
			info.LoopTime = 1.0f;
			info.ImageWidth = 406;
			info.ImageHeight = 255;
			info.LoadingImagePath = std::vector<std::string>{ "GUI/Loading/Bicycle/0.png","GUI/Loading/Bicycle/1.png","GUI/Loading/Bicycle/2.png","GUI/Loading/Bicycle/3.png" };

			sceneManager.SetSpawnPosition(glm::vec3(-12.0143, 0, -16.5855));
			sceneManager.LoadScene<YardScene>(info);

			globalVariable.taskState[5] = TaskState::Finished;
		};

		eventManager.RegisterForEvent<ForestSceneGoHomeInteractEvent>(gohomeInteractCallback);

		std::function<void(const ForestSceneAppleInteractEvent& event)> appleInteractCallback =
			[scene = this, &appleModel = appleModel, &itemStatus = itemStatus](const ForestSceneAppleInteractEvent& event) {
			appleModel.SetMeshTransform(0, 0, glm::mat4(0.0));
			itemStatus[0] = true;
		};

		eventManager.RegisterForEvent<ForestSceneAppleInteractEvent>(appleInteractCallback);

		std::function<void(const ForestSceneMushroomInteractEvent& event)> mushroomInteractCallback =
			[scene = this, &mushroomModel = mushroomModel, &itemStatus = itemStatus](const ForestSceneMushroomInteractEvent& event) {
			mushroomModel.SetMeshTransform(0, 0, glm::mat4(0.0));
			itemStatus[1] = true;
		};

		eventManager.RegisterForEvent<ForestSceneMushroomInteractEvent>(mushroomInteractCallback);

		std::function<void(const ForestSceneWoodInteractEvent& event)> woodInteractCallback =
			[scene = this, &woodModel = woodModel, &itemStatus = itemStatus](const ForestSceneWoodInteractEvent& event) {
			woodModel.SetMeshTransform(0, 0, glm::mat4(0.0));
			itemStatus[2] = true;
		};

		eventManager.RegisterForEvent<ForestSceneWoodInteractEvent>(woodInteractCallback);
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