#pragma once
#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Renderer/Renderer.h>

struct SceneInfo {
	World* world;
	Renderer* renderer;
};

class Scene
{
public:
	Scene(const SceneInfo& info) :world(*info.world), renderer(*info.renderer), prefabsSteup(false) { };
	virtual void Setup() = 0;
protected:
	virtual void setupPrefab() = 0;
	bool prefabsSteup;

	World& world;
	Renderer& renderer;
};
