#pragma once
#include <thirdEngine/Framework/System.h>
#include <thirdEngine/Input/Input.h>

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

	float horizontalRad;
	float verticalRad;
};
