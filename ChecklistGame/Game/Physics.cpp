#include <algorithm>

#include "Game/Event/CollisionEvent.h"
#include "Physics.h"

Physics::Physics(b2World* dynamicsWorld, EventManager& eventManager)
	:dynamicsWorld(dynamicsWorld),
	eventManager(eventManager)
{ }

void Physics::BeginContact(b2Contact* contact)
{
	b2Fixture* objA = contact->GetFixtureA();
	b2Fixture* objB = contact->GetFixtureB();

	eid_t e1 = *reinterpret_cast<eid_t*>(objA->GetBody()->GetUserData().pointer);
	eid_t e2 = *reinterpret_cast<eid_t*>(objB->GetBody()->GetUserData().pointer);

	if (e1 < 0 || e2 < 0)
	{
		return;
	}

	eid_t emin = std::min(e1, e2);
	eid_t emax = std::max(e1, e2);

	this->handleContact(e1, e2, contact->GetManifold(), CollisionResponseType::Begin);
}

void Physics::EndContact(b2Contact* contact)
{
	b2Fixture* objA = contact->GetFixtureA();
	b2Fixture* objB = contact->GetFixtureB();

	eid_t e1 = *reinterpret_cast<eid_t*>(objA->GetBody()->GetUserData().pointer);
	eid_t e2 = *reinterpret_cast<eid_t*>(objB->GetBody()->GetUserData().pointer);
	if (e1 == 0 || e2 == 0)
	{
		return;
	}

	eid_t emin = std::min(e1, e2);
	eid_t emax = std::max(e1, e2);

	this->handleContact(e1, e2, contact->GetManifold(), CollisionResponseType::End);
}

void Physics::handleContact(eid_t e1, eid_t e2, b2Manifold* contactManifold, CollisionResponseType type)
{
	CollisionEvent evt;
	evt.e1 = e1;
	evt.e2 = e2;
	evt.collisionManifold = contactManifold;
	evt.type = type;
	eventManager.SendEvent(evt);
}