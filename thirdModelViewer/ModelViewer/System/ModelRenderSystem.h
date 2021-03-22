#pragma once
#include <thirdEngine/Framework/System.h>

class Renderer;

class ModelRenderSystem :public System
{
public:
	ModelRenderSystem(World& world, Renderer& renderer);

protected:
	void updateEntity(float dt, eid_t entity);
private:
	Renderer& renderer;
};
