#pragma once
#include <vector>

#include <thirdEngine/Renderer/Shader.h>
#include <thirdEngine/Renderer/Model.h>
#include <thirdEngine/Renderer/ModelLoader.h>
#include <thirdEngine/Framework/EventManager.h>

#include "Game/Storyboard/Storyboard.h"
#include "Game/Storyboard/StoryboardLoader.h"

#include "Game/UIElement/Home/StartMenu.h"
#include "Game/UIElement/Home/Letter.h"

#include "Game/Scene/Scene.h"

class YardScene :public Scene
{
public:
	YardScene(const SceneInfo& info, EventManager& evtMgr)
		:Scene(info), eventManager(evtMgr) { }
	void Setup();
protected:
	void setupPrefab();
private:
	struct EntityId
	{
		eid_t player;
		eid_t camera;
		eid_t storyboard;
	};

	EntityId entityId;

	ModelLoader modelLoader;
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;
	StoryboardLoader storyboardLoader;

	Shader skinnedShader;
	Shader plainShader;
	Shader skyboxShader;

	Prefab yardPrefab;
	Prefab playerPrefab;
	Prefab cameraPrefab;
	Prefab storyboardPrefab;

	EventManager& eventManager;

	Renderer::RenderableHandle skybox;

	std::vector<Storyboard> storyboards;

	/* GUI */
	std::shared_ptr<StartMenu> startMenu;
	std::shared_ptr<Letter> letter;
	UIRenderer::UIElementHandle startMenuHandle;
	UIRenderer::UIElementHandle letterHandle;
};
