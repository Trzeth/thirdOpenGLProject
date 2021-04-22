#pragma once
#include <thirdEngine/Framework/System.h>

class CollisionUpdateSystem :public System
{
public:
	CollisionUpdateSystem(World& world);

protected:
	void updateEntity(float dt, eid_t entity);
};
