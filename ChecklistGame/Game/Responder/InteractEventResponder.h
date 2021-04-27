#pragma once
#include <thirdEngine/Framework/ComponentBitmask.h>

class World;
class EventManager;
class CollisionEvent;

class InteractEventResponder
{
public:
	InteractEventResponder(World& world, EventManager& eventManager);
	~InteractEventResponder();
	void handleCollisionEvent(const CollisionEvent& event);
private:
	World& world;
	EventManager& eventManager;
	ComponentBitmask requiredComponents1;
	ComponentBitmask requiredComponents2;
};
