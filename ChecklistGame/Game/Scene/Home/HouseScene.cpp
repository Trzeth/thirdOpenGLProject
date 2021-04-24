#include "HouseScene.h"

#include "Game/Component/TransformComponent.h"
#include "Game/Component/ModelRenderComponent.h"
#include "Game/Component/CameraComponent.h"
#include "Game/Component/PlayerComponent.h"
#include "Game/Component/StoryboardDirectorComponent.h"
#include "Game/Component/CollisionComponent.h"
#include "Game/Component/RigidbodyMotorComponent.h"

void HouseScene::Setup()
{
	setupPrefab();

	eid_t player = world.ConstructPrefab(playerPrefab);
	eid_t camera = world.ConstructPrefab(cameraPrefab);

	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(player);
	playerComponent->data.camera = camera;
	playerComponent->SetControlState(PlayerControlState::Normal);

	CameraComponent* cameraComponent = world.GetComponent<CameraComponent>(camera);
	cameraComponent->isEnable = true;

	world.ConstructPrefab(housePrefab);
}

void HouseScene::setupPrefab()
{
	if (prefabsSteup)
		return;

	skinnedShader = shaderLoader.BuildFromFile("Shaders/skinnedShader.vs", "Shaders/skinnedShader.fs");
	plainShader = shaderLoader.BuildFromFile("Shaders/plainShader.vs", "Shaders/plainShader.fs");

	/* House */
	{
		glm::mat4 houseModelMat4(1.0f);
		houseModelMat4 *= glm::scale(houseModelMat4, glm::vec3(0.4f));

		Model houseModel = modelLoader.LoadModel("Resources/Room/Room.FBX", houseModelMat4);
		Renderer::ModelHandle houseModelHandle = renderer.GetModelHandle(houseModel);

		housePrefab.SetName("YardModel");

		housePrefab.AddConstructor(new TransformComponentConstructor());
		housePrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, houseModelHandle, plainShader));

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

		housePrefab.AddConstructor(new CollisionComponentConstructor(dynamicsWorld, CollisionConstructorInfo(bodyDef, fixtures)));
	}

	/* Player */
	{
		glm::mat4 playerModelMat4(1.0f);
		playerModelMat4 *= glm::scale(playerModelMat4, glm::vec3(0.5f));
		playerModelMat4 *= glm::mat4_cast(glm::angleAxis(glm::radians(180.0f), Transform::UP));

		std::shared_ptr<Model> playerWalkModel = std::make_shared<Model>(modelLoader.LoadModel("Resources/Walk.DAE", playerModelMat4));
		std::shared_ptr<Model> playerPickLetterModel = std::make_shared<Model>(modelLoader.LoadModel("Resources/PickLetter.DAE", playerModelMat4));
		std::shared_ptr<Model> playerPutLetterModel = std::make_shared<Model>(modelLoader.LoadModel("Resources/PutLetter.DAE", playerModelMat4));
		std::shared_ptr<Model> playerTurnAroundModel = std::make_shared<Model>(modelLoader.LoadModel("Resources/TurnAround.DAE", playerModelMat4));

		models.push_back(playerWalkModel);
		models.push_back(playerPickLetterModel);
		models.push_back(playerPutLetterModel);
		models.push_back(playerTurnAroundModel);

		playerPrefab.SetName("Player");
		playerPrefab.AddConstructor(new TransformComponentConstructor());

		PlayerComponent::Data playerData;
		playerData.walk = renderer.GetModelHandle(*playerWalkModel);
		playerData.pickLetter = renderer.GetModelHandle(*playerPickLetterModel);
		playerData.putLetter = renderer.GetModelHandle(*playerPutLetterModel);
		playerData.turnAround = renderer.GetModelHandle(*playerTurnAroundModel);

		playerPrefab.AddConstructor(new PlayerComponentConstructor(playerData));
		playerPrefab.AddConstructor(new ModelRenderComponentConstructor(renderer, playerData.walk, skinnedShader));
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

	prefabsSteup = true;
}

void HouseScene::Finish()
{
	renderer.GenVAO();
}