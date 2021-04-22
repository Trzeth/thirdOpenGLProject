#pragma once
#include <thirdEngine/Framework/Event.h>

#include "Game/Physics.h"

class CollisionEvent :public Event
{
public:
	eid_t e1;
	eid_t e2;
	b2Manifold* collisionManifold;
	CollisionResponseType type;
};