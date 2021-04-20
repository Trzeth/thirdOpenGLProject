#pragma once
#include <thirdEngine/Framework/System.h>

class Input;
class EventManager;

class PlayerInputSystem :public System
{
public:
	PlayerInputSystem(World& world, Input& input, EventManager& eventManager);
protected:
	void updateEntity(float dt, eid_t entity);
private:
	Input& input;
	EventManager& eventManager;

	float horizontalRad = 0.0f;

	float verticalRad = 0.79f;
};
