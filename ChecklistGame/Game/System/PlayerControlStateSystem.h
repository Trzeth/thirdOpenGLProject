#pragma once
#include <thirdEngine/Framework/System.h>

class Window;

class PlayerControlStateSystem :public System
{
public:
	PlayerControlStateSystem(World& world, Window& window);
protected:
	void updateEntity(float dt, eid_t entity);
private:
	Window& window;
};
