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
#include "Game/UIElement/Checklist.h"

#include "Game/Scene/Scene.h"

class YardScene :public Scene
{
public:
	YardScene(const SceneInfo& info) :Scene(info) { }

	void Setup();

	void Finish();
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

	Renderer::RenderableHandle skybox;

	std::vector<Storyboard> storyboards;

	std::vector<std::shared_ptr<Model>> models;

	/* GUI */
	std::shared_ptr<StartMenu> startMenu;
	std::shared_ptr<Letter> letter;
	std::shared_ptr<Checklist> checklist;
	UIRenderer::UIElementHandle startMenuHandle;
	UIRenderer::UIElementHandle letterHandle;
	UIRenderer::UIElementHandle checklistHandle;
};
