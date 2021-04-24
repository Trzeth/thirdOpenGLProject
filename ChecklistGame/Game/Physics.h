#pragma once
#include <unordered_map>
#include <box2d/box2d.h>

#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Framework/EventManager.h>

enum class CollisionResponseType
{
	//Began
	Begin,
	//Ended
	End
};

class Physics :public b2ContactListener
{
public:
	Physics(b2World* dynamicsWorld, EventManager& eventManager);

	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);

private:
	void handleContact(eid_t e1, eid_t e2, b2Manifold* contactManifold, CollisionResponseType type);

	EventManager& eventManager;

	b2World* dynamicsWorld;
};