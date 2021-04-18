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
	virtual void Setup() = 0;

	/*!
	 * @brief Run on main thread
	 * for GenVAO
	*/
	virtual void Finish() { };
protected:
	virtual void setupPrefab() = 0;
	bool prefabsSteup;

	World& world;
	Renderer& renderer;
	UIRenderer& uiRenderer;
};
