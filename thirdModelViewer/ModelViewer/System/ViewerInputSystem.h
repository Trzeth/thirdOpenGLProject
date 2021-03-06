#pragma once
#include <thirdEngine/Framework/System.h>

class Input;
class EventManager;

constexpr double CAMERASPEED = 0.1;

class ViewerInputSystem :public System
{
public:
	ViewerInputSystem(World& world, Input& input, EventManager& eventManager);
protected:
	void updateEntity(float dt, eid_t entity);
private:
	Input& input;
	EventManager& eventManager;

	float horizontalRad;
	float verticalRad;
};
