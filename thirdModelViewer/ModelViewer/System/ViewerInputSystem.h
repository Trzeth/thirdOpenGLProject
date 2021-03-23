#pragma once
#include <thirdEngine/Framework/System.h>

class Input;
class EventManager;

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
