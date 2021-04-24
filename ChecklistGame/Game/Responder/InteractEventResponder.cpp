#include "InteractEventResponder.h"

#include <thirdEngine/Framework/EventManager.h>

#include "Game/Component/InteractComponent.h"
#include "Game/Component/PlayerComponent.h"
#include "Game/Event/CollisionEvent.h"

InteractEventResponder::InteractEventResponder(World& world, EventManager& eventManager)
	:world(world), eventManager(eventManager)
{
	requiredComponents1.SetBit(world.GetComponentId<PlayerComponent>(), true);
	requiredComponents2.SetBit(world.GetComponentId<InteractComponent>(), true);
	eventManager.RegisterForEvent<CollisionEvent>(std::bind(&InteractEventResponder::handleCollisionEvent, this, std::placeholders::_1));
}

void InteractEventResponder::handleCollisionEvent(const CollisionEvent& event)
{
	eid_t e1 = event.e1, e2 = event.e2;

	if (!world.OrderEntities(e1, e2, requiredComponents1, requiredComponents2)) {
		return;
	}

	if (event.type != CollisionResponseType::Begin) {
		return;
	}

	InteractComponent* interactComponent = world.GetComponent<InteractComponent>(e2);

	if (!interactComponent)return;

	eventManager.SendEvent(interactComponent->data.eventHashCode);
}