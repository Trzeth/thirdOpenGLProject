#pragma once
#include <thirdEngine/Renderer/Shader.h>
#include <thirdEngine/Renderer/Model.h>
#include <thirdEngine/Renderer/ModelLoader.h>
#include <thirdEngine/Framework/EventManager.h>

#include "Game/Responder/InteractEventResponder.h"

#include "Game//UIElement/Home/ItemInfo.h"
#include "Game/UIElement/Home/Ending.h"

#include "Game/Scene/Scene.h"

class HouseScene :public Scene
{
public:
	HouseScene(const SceneInfo& info) :Scene(info) { }

	void Setup();

	void Finish();

	void PreDestruct();
protected:
	void setupPrefab();
private:
	ModelLoader modelLoader;
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;

	Shader lightingShader;

	Prefab housePrefab;
	Prefab playerPrefab;
	Prefab cameraPrefab;

	Prefab songbingPrefab;
	Prefab bangbangtangPrefab;
	Prefab giftPrefab;

	Prefab doorInteractPrefab;

	std::shared_ptr<ItemInfo> iteminfo;
	UIRenderer::UIElementHandle iteminfoHandle;

	std::shared_ptr<Ending> ending;
	UIRenderer::UIElementHandle endingHandle;

	std::shared_ptr<InteractEventResponder> interactEventResponder;
};