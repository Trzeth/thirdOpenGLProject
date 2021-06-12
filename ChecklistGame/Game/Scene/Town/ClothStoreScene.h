#pragma once
#include <thirdEngine/Renderer/Shader.h>
#include <thirdEngine/Renderer/Model.h>
#include <thirdEngine/Renderer/ModelLoader.h>
#include <thirdEngine/Framework/EventManager.h>

#include "Game/Responder/InteractEventResponder.h"

#include "Game/Scene/Scene.h"

#include "Game/UIElement/Town/ClothStoreUI.h"

class ClothStoreScene :public Scene
{
public:
	ClothStoreScene(const SceneInfo& info) :Scene(info) { }

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

	Model playerWalkModel;
	Shader skyboxShader;

	Prefab skyboxPrefab;
	Prefab playerPrefab;
	Prefab cameraPrefab;

	std::shared_ptr<InteractEventResponder> interactEventResponder;

	/* GUI */
	std::shared_ptr<ClothStoreUI> clothStoreUI;
	UIRenderer::UIElementHandle clothStoreUIHandle;
};
