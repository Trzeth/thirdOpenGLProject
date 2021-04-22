#pragma once
#include <thirdEngine/Framework/System.h>

class RigidbodyMotorSystem :public System
{
public:
	RigidbodyMotorSystem(World& world);
protected:
	void updateEntity(float dt, eid_t entity);
};
