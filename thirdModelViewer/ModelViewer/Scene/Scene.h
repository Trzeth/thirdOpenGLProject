#pragma once
#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Renderer/Renderer.h>
#include <thirdEngine/Renderer/UI/UIRenderer.h>

struct SceneInfo {
	World* world;
	Renderer* renderer;
	UIRenderer* uiRenderer;
};

class Scene
{
public:
	Scene(const SceneInfo& info) :world(*info.world), renderer(*info.renderer), uiRenderer(*info.uiRenderer), prefabsSteup(false) { };
	virtual ~Scene() = default;
	virtual void Setup() = 0;
protected:
	virtual void setupPrefab() = 0;
	bool prefabsSteup;

	World& world;
	Renderer& renderer;
	UIRenderer& uiRenderer;
};
