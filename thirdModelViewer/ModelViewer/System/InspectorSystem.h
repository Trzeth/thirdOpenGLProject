#pragma once
#include <thirdEngine/Framework/System.h>

#include <thirdEngine/Renderer/Model.h>
#include <thirdEngine/Renderer/ModelLoader.h>
class Renderer;

constexpr float RESIZETHRESHOLD = 1.0f;

class InspectorSystem :public System
{
public:
	InspectorSystem(World& world, Renderer& renderer);

protected:
	void updateEntity(float dt, eid_t entity);

private:
	Renderer& renderer;
	float lastResizeTime;

	ModelLoader modelLoader;
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;
};
