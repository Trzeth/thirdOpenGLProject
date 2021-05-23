#pragma once
#include <thirdEngine/Renderer/Shader.h>
#include <thirdEngine/Renderer/Model.h>
#include <thirdEngine/Renderer/ModelLoader.h>
#include <thirdEngine/Framework/EventManager.h>

#include "Game/Responder/InteractEventResponder.h"

#include "Game/Scene/Scene.h"

class ForestScene :public Scene
{
public:
	ForestScene(const SceneInfo& info) :Scene(info) { }

	void Setup();

	void Finish();

	void PreDestruct();
protected:
	void setupPrefab();
private:
	ModelLoader modelLoader;
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;

	Shader skinnedShader;
	Shader plainShader;

	Prefab housePrefab;
	Prefab playerPrefab;
	Prefab cameraPrefab;

	Prefab doorInteractPrefab;

	std::shared_ptr<InteractEventResponder> interactEventResponder;
};