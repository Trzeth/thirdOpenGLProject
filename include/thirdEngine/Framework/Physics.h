#pragma once
#include <bullet/btBulletDynamicsCommon.h>

#include <unordered_map>

#include "World.h"
#include "EventManager.h"

enum CollisionResponseType
{
	CollisionResponseType_Began,
	CollisionResponseType_Ended
};

template < >
struct std::hash<std::pair<eid_t, eid_t>>
{
public:
	size_t operator()(std::pair<eid_t, eid_t> epair) const throw() {
		// I found this on the 'net, must be good right
		return std::hash<eid_t>()(epair.first) * 31 + std::hash<eid_t>()(epair.second);
	}
};

class Physics
{
public:
	Physics(btDynamicsWorld* dynamicsWorld, EventManager& eventManager);

	void FixedUpdate(btDynamicsWorld* world, float timeStep);

	btPersistentManifold* GetContact(eid_t e1, eid_t e2);
private:
	void handleContact(eid_t e1, eid_t e2, btPersistentManifold* contactManifold, CollisionResponseType type);

	struct PhysicsContact
	{
		PhysicsContact() : lastContactFrame(UINT64_MAX), contactManifold(nullptr) { }
		uint64_t lastContactFrame;
		btPersistentManifold* contactManifold;
	};

	EventManager& eventManager;

	std::unordered_map<std::pair<eid_t, eid_t>, PhysicsContact> contacts;
	btDynamicsWorld* dynamicsWorld;
	uint64_t currentFrame;

	const static unsigned framesBeforeUncaching;
};