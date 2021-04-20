#pragma once
#include <thirdEngine/Framework/System.h>

class Renderer;

class PlayerAnimationStateSystem :public System
{
public:
	PlayerAnimationStateSystem(World& world, Renderer& renderer);
protected:
	void updateEntity(float dt, eid_t entity);
private:
	Renderer& renderer;
};
