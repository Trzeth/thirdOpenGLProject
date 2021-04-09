#pragma once
#include <thirdEngine/Renderer/Shader.h>
#include <thirdEngine/Renderer/Model.h>
#include <thirdEngine/Renderer/ModelLoader.h>
#include <thirdEngine/Framework/EventManager.h>
#include "Scene.h"

class MaterialWrapper;

class DefaultScene :public Scene
{
public:
	DefaultScene(const SceneInfo& info, EventManager& evtMgr)
		:Scene(info), eventManager(evtMgr) { }
	void Setup();
protected:
	void setupPrefab();
private:
	std::vector<MaterialWrapper> getMaterialList();

	ModelLoader modelLoader;
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;

	Shader shader;
	Shader skyboxShader;

	EventManager& eventManager;

	//Viewer of the object
	Prefab viewerPrefab;

	//UI Entity
	Prefab inspectorPrefab;
	Prefab cameraPrefab;

	Renderer::RenderableHandle skybox;
};
