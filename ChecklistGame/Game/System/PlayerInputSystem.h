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

	const float PI = 3.14159265358979323846264338327950288;
	float horizontalRad = -PI / 2;
	float verticalRad = PI / 4;
};
