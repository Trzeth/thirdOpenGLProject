#pragma once
#include <thirdEngine/Framework/System.h>

class Renderer;

class CameraSystem :public System
{
public:
	CameraSystem(World& world, Renderer& renderer);

protected:
	void updateEntity(float dt, eid_t entity);

private:
	Renderer& renderer;
};
