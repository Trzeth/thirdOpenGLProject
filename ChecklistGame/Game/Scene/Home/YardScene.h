#pragma once
#include <thirdEngine/Renderer/Shader.h>
#include <thirdEngine/Renderer/Model.h>
#include <thirdEngine/Renderer/ModelLoader.h>
#include <thirdEngine/Framework/EventManager.h>
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
	ModelLoader modelLoader;
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;

	Shader skinnedShader;
	Shader plainShader;
	Shader skyboxShader;

	Prefab yardPrefab;
	Prefab playerPrefab;
	Prefab cameraPrefab;

	EventManager& eventManager;

	Renderer::RenderableHandle skybox;
};
