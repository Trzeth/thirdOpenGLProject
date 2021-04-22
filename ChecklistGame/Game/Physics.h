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

template <>
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
	Physics(b2World* dynamicsWorld, EventManager& eventManager);

	void Update(float timeStep);

	b2Manifold* GetContact(eid_t e1, eid_t e2);
private:
	void handleContact(eid_t e1, eid_t e2, b2Manifold* contactManifold, CollisionResponseType type);

	struct PhysicsContact
	{
		PhysicsContact() : lastContactFrame(UINT64_MAX), contactManifold(nullptr) { }
		uint64_t lastContactFrame;
		b2Manifold* contactManifold;
	};

	EventManager& eventManager;

	std::unordered_map<std::pair<eid_t, eid_t>, PhysicsContact> contacts;
	b2World* dynamicsWorld;
	uint64_t currentFrame;

	const static unsigned framesBeforeUncaching;
};