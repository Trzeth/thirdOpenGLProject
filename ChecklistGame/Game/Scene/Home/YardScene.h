#pragma once
#include <vector>

#include <thirdEngine/Renderer/Shader.h>
#include <thirdEngine/Renderer/Model.h>
#include <thirdEngine/Renderer/ModelLoader.h>
#include <thirdEngine/Framework/EventManager.h>

#include "Game/Responder/InteractEventResponder.h"

#include "Game/Storyboard/Storyboard.h"
#include "Game/Storyboard/StoryboardLoader.h"

#include "Game/UIElement/Home/StartMenu.h"
#include "Game/UIElement/Home/Letter.h"
#include "Game/UIElement/Home/BrushLoading.h"
#include "Game/UIElement/Home/BrushSelector.h"
#include "Game/UIElement/Home/WaterFlower.h"
#include "Game/UIElement/Checklist.h"

#include "Game/Scene/Scene.h"

class YardScene :public Scene
{
public:
	YardScene(const SceneInfo& info) :Scene(info) { spawnPosition = info.spawnPosition; }

	void Setup();

	void Finish();

	void PreDestruct();
protected:
	void setupPrefab();
private:
	struct EntityId
	{
		eid_t player;
		eid_t camera;
		eid_t storyboard;
	};

	Transform spawnPosition;

	EntityId entityId;

	ModelLoader modelLoader;
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;
	StoryboardLoader storyboardLoader;

	Shader skinnedShader;
	Shader plainShader;
	Shader skyboxShader;

	Prefab skyboxPrefab;
	Prefab yardPrefab;
	Prefab playerPrefab;
	Prefab cameraPrefab;
	Prefab storyboardPrefab;

	Prefab brushInteractPrefab;
	Prefab waterPotInteractPrefab;
	Prefab doorInteractPrefab;
	Prefab bikeInteractPrefab;

	std::shared_ptr<InteractEventResponder> interactEventResponder;

	std::vector<Storyboard> storyboards;

	/* For change fence */
	Model yardModel;

	Texture fenceTexture[4];

	/* GUI */
	std::shared_ptr<StartMenu> startMenu;
	std::shared_ptr<Letter> letter;
	std::shared_ptr<BrushLoading> brushLoading;
	std::shared_ptr<BrushSelector> brushSelector;
	std::shared_ptr<WaterFlower> waterFlower;

	UIRenderer::UIElementHandle startMenuHandle;
	UIRenderer::UIElementHandle letterHandle;
	UIRenderer::UIElementHandle brushLoadingHandle;
	UIRenderer::UIElementHandle brushSelectorHandle;
	UIRenderer::UIElementHandle waterFlowerHandle;

	/* Global GUI */
	std::shared_ptr<Checklist> checklist;
	UIRenderer::UIElementHandle checklistHandle;
};
