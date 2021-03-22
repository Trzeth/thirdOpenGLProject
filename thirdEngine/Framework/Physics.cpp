#include <algorithm>

#include "CollisionEvent.h"
#include "Physics.h"

const unsigned int Physics::framesBeforeUncaching = 60 * 30;

static void bulletTickCallback(btDynamicsWorld* world, btScalar timeStep) {
	Physics* physics = static_cast<Physics*>(world->getWorldUserInfo());
	physics->FixedUpdate(world, timeStep);
}

Physics::Physics(btDynamicsWorld* dynamicsWorld, EventManager& eventManager)
	:dynamicsWorld(dynamicsWorld),
	eventManager(eventManager),
	currentFrame(0)
{
	dynamicsWorld->setInternalTickCallback(bulletTickCallback, static_cast<void*>(this));
}

void Physics::FixedUpdate(btDynamicsWorld* world, float timeStep)
{
	assert(dynamicsWorld == world);
	this->currentFrame++;

	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i != numManifolds; i++) {
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* objA = contactManifold->getBody0();
		const btCollisionObject* objB = contactManifold->getBody1();

		if (objA->getUserPointer() == nullptr ||
			objB->getUserPointer() == nullptr ||
			contactManifold->getNumContacts() <= 0)
		{
			continue;
		}

		eid_t e1 = *(eid_t*)objA->getUserPointer();
		eid_t e2 = *(eid_t*)objB->getUserPointer();

		eid_t emin = std::min(e1, e2);
		eid_t emax = std::max(e1, e2);

		PhysicsContact& contact = contacts[std::make_pair(emin, emax)];
		if (contact.lastContactFrame != currentFrame - 1) {
			this->handleContact(e1, e2, contactManifold, CollisionResponseType_Began);
		}

		contact.lastContactFrame = currentFrame;
		contact.contactManifold = contactManifold;
	}

	auto iter = contacts.begin();
	while (iter != contacts.end()) {
		eid_t e1 = iter->first.first;
		eid_t e2 = iter->first.second;

		PhysicsContact& contact = iter->second;

		if (contact.lastContactFrame == currentFrame - 1) {
			this->handleContact(e1, e2, contact.contactManifold, CollisionResponseType_Ended);
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

btPersistentManifold* Physics::GetContact(eid_t e1, eid_t e2)
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

void Physics::handleContact(eid_t e1, eid_t e2, btPersistentManifold* contactManifold, CollisionResponseType type)
{
	CollisionEvent evt;
	evt.e1 = e1;
	evt.e2 = e2;
	evt.collisionManifold = contactManifold;
	evt.type = type;
	eventManager.SendEvent(evt);
}