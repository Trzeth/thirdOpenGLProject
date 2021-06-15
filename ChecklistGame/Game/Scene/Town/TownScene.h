#pragma once
#include <thirdEngine/Renderer/Shader.h>
#include <thirdEngine/Renderer/Model.h>
#include <thirdEngine/Renderer/ModelLoader.h>
#include <thirdEngine/Framework/EventManager.h>

#include "Game/Responder/InteractEventResponder.h"
#include "Game/UIElement/BikeMap.h"

#include "Game/Scene/Scene.h"

class TownScene :public Scene
{
public:
	TownScene(const SceneInfo& info) :Scene(info) { spawnPosition = info.spawnPosition; }

	void Setup();

	void Finish();

	void PreDestruct();
protected:
	void setupPrefab();
private:
	ModelLoader modelLoader;
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;

	Transform spawnPosition;

	Shader lightingShader;
	Shader skyboxShader;

	Prefab townPrefab;
	Prefab skyboxPrefab;
	Prefab playerPrefab;
	Prefab cameraPrefab;

	Prefab shopDoorInteractPrefab;
	Prefab clothStoreDoorInteractPrefab;

	std::shared_ptr<InteractEventResponder> interactEventResponder;

	std::shared_ptr<BikeMap> bikeMap;
	UIRenderer::UIElementHandle bikeMapHandle;
};