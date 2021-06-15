#pragma once
#include <thirdEngine/Renderer/Shader.h>
#include <thirdEngine/Renderer/Model.h>
#include <thirdEngine/Renderer/ModelLoader.h>
#include <thirdEngine/Framework/EventManager.h>

#include "Game/Responder/InteractEventResponder.h"
#include "Game/UIElement/Forest/DeerNPCConversation.h"

#include "Game/Scene/Scene.h"

class ForestScene :public Scene
{
public:
	ForestScene(const SceneInfo& info) :Scene(info)
	{
		for (int i = 0; i != 4; i++)
			itemStatus.push_back(false);
	}

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
	Shader skyboxShader;

	Prefab forestPrefab;
	Prefab skyboxPrefab;
	Prefab playerPrefab;
	Prefab cameraPrefab;

	Prefab npcInteractPrefab;
	Prefab applePrefab;
	Prefab mushroomPrefab;
	Prefab woodPrefab;

	Model appleModel;
	Model woodModel;
	Model mushroomModel;

	std::vector<bool> itemStatus;

	/* GUI */
	std::shared_ptr<DeerNPCConversation> npcConversation;
	UIRenderer::UIElementHandle npcConversationHandle;

	std::shared_ptr<InteractEventResponder> interactEventResponder;
};