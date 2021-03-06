#pragma once
#include <thirdEngine/Renderer/Shader.h>
#include <thirdEngine/Renderer/Model.h>
#include <thirdEngine/Renderer/ModelLoader.h>
#include <thirdEngine/Framework/EventManager.h>
#include "Scene.h"

class MaterialWrapper;

class TestScene :public Scene
{
public:
	TestScene(const SceneInfo& info, EventManager& evtMgr)
		:Scene(info), eventManager(evtMgr) { }
	void Setup();
protected:
	void setupPrefab();

private:
	ModelLoader modelLoader;
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;

	Shader shader;

	EventManager& eventManager;

	//Viewer of the object
	Prefab viewerPrefab;

	//UI Entity
	Prefab inspectorPrefab;
	Prefab cameraPrefab;
};