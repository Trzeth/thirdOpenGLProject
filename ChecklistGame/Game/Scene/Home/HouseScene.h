#pragma once
#include <thirdEngine/Renderer/Shader.h>
#include <thirdEngine/Renderer/Model.h>
#include <thirdEngine/Renderer/ModelLoader.h>
#include <thirdEngine/Framework/EventManager.h>
#include "Game/Scene/Scene.h"

class HouseScene :public Scene
{
public:
	HouseScene(const SceneInfo& info, EventManager& evtMgr)
		:Scene(info), eventManager(evtMgr) { }
	void Setup();
protected:
	void setupPrefab();
private:
	ModelLoader modelLoader;
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;

	Shader shader;
	Shader skyboxShader;

	EventManager& eventManager;

	Renderer::RenderableHandle skybox;
};