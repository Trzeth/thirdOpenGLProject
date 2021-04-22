#include <algorithm>

#include "Game/Event/CollisionEvent.h"
#include "Physics.h"

const unsigned int Physics::framesBeforeUncaching = 60 * 30;

/*
static void bulletTickCallback(btDynamicsWorld* world, btScalar timeStep) {
	Physics* physics = static_cast<Physics*>(world->getWorldUserInfo());
	physics->FixedUpdate(world, timeStep);
}
*/

Physics::Physics(b2World* dynamicsWorld, EventManager& eventManager)
	:dynamicsWorld(dynamicsWorld),
	eventManager(eventManager),
	currentFrame(0)
{ }

void Physics::Update(float timeStep)
{
	this->currentFrame++;

	for (b2Contact* c = dynamicsWorld->GetContactList(); c; c = c->GetNext())
	{
		b2Fixture* objA = c->GetFixtureA();
		b2Fixture* objB = c->GetFixtureA();

		eid_t e1 = *(eid_t*)&objA->GetUserData();
		eid_t e2 = *(eid_t*)&objB->GetUserData();

		if (e1 == 0 || e2 == 0 ||
			c->GetManifold()->pointCount <= 0) {
			continue;
		}

		eid_t emin = std::min(e1, e2);
		eid_t emax = std::max(e1, e2);

		PhysicsContact& contact = contacts[std::make_pair(emin, emax)];
		if (contact.lastContactFrame != currentFrame - 1) {
			this->handleContact(e1, e2, c->GetManifold(), CollisionResponseType::Begin);
		}

		contact.lastContactFrame = currentFrame;
		contact.contactManifold = c->GetManifold();
	}

	auto iter = contacts.begin();
	while (iter != contacts.end()) {
		eid_t e1 = iter->first.first;
		eid_t e2 = iter->first.second;

		PhysicsContact& contact = iter->second;

		if (contact.lastContactFrame == currentFrame - 1) {
			this->handleContact(e1, e2, contact.contactManifold, CollisionResponseType::End);
		}

		if (currentFrame - contact.lastContactFrame >= framesBeforeUncaching) {
			iter = this->contacts.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

b2Manifold* Physics::GetContact(eid_t e1, eid_t e2)
{
	eid_t emin = std::min(e1, e2);
	eid_t emax = std::max(e1, e2);

	auto iter = contacts.find(std::make_pair(emin, emax));
	if (iter == contacts.end())
		return nullptr;

	PhysicsContact& contact = iter->second;
	if (contact.lastContactFrame != currentFrame)
		return nullptr;

	return contact.contactManifold;
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