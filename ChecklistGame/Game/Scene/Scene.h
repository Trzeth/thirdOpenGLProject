#pragma once
#include <box2d/box2d.h>
#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Renderer/Renderer.h>
#include <thirdEngine/Renderer/UI/UIRenderer.h>

struct SceneInfo {
	World* world;
	Renderer* renderer;
	UIRenderer* uiRenderer;
	b2World* dynamicsWorld;
};

class Scene
{
public:
	Scene(const SceneInfo& info) :world(*info.world), renderer(*info.renderer), uiRenderer(*info.uiRenderer), dynamicsWorld(*info.dynamicsWorld), prefabsSteup(false) { };
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
	b2World& dynamicsWorld;
};
