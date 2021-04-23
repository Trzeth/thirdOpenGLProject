#pragma once
#include <box2d/box2d.h>
#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Renderer/Renderer.h>
#include <thirdEngine/Renderer/UI/UIRenderer.h>
#include <thirdEngine/Input/Input.h>
#include <thirdEngine/Framework/EventManager.h>

class GlobalVariable;

struct SceneInfo {
	World* world;
	Renderer* renderer;
	UIRenderer* uiRenderer;
	b2World* dynamicsWorld;
	Input* input;
	EventManager* eventManager;
	GlobalVariable* globalVariable;
};

class Scene
{
public:
	Scene(const SceneInfo& info)
		:world(*info.world), renderer(*info.renderer), uiRenderer(*info.uiRenderer),
		input(*info.input), dynamicsWorld(*info.dynamicsWorld), eventManager(*info.eventManager),
		globalVariable(*info.globalVariable), prefabsSteup(false)
	{ };

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
	Input& input;
	b2World& dynamicsWorld;
	EventManager& eventManager;
	GlobalVariable& globalVariable;
};
