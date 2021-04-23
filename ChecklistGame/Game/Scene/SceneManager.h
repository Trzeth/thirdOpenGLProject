#pragma once
#include <memory>
#include <vector>
#include <string>
#include <thread>

#include <GLFW/glfw3.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Renderer/Texture.h>

#include "Game/Event/LoadSceneEndEvent.h"
#include "Game/Event/LoadSceneStartEvent.h"
#include "Game/Window.h"

#include "Scene.h"

enum class Effect {
	CircleIn,
	CirleOut,
	FadeIn,
	FadeOut,
	None
};

struct LoadingScreenInfo
{
	LoadingScreenInfo()
		:LoadingImagePath(), LoopTime(), EnterEffect(), ExitEffect()
	{ }

	LoadingScreenInfo(std::vector<std::string> loadingImagePath, float loopTime, Effect enter = Effect::None, Effect exit = Effect::None)
		:LoadingImagePath(loadingImagePath), LoopTime(loopTime), EnterEffect(enter), ExitEffect(exit)
	{ }

	std::vector<std::string> LoadingImagePath;
	float LoopTime;

	Effect EnterEffect;
	Effect ExitEffect;
};

class SceneManager
{
public:
	SceneManager(Window& window, SceneInfo& sceneInfo)
		:sceneInfo(sceneInfo), eventManager(*sceneInfo.eventManager),
		window(window), loadingWindow(nullptr)
	{
		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
		loadingWindow = glfwCreateWindow(1, 1, "loadingThread", NULL, window.GetWindow());
		glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
		assert(loadingWindow);
	}

	template<class T>
	void LoadScene(LoadingScreenInfo info);

	void LoadFinish();

	void Update(float dt);

	/* Draw Loading Screen */
	void Draw();
private:
	SceneInfo& sceneInfo;
	EventManager& eventManager;
	Window& window;
	std::unique_ptr<Scene> currentScene;
	GLFWwindow* loadingWindow;
	std::thread loadingThread;

	LoadingScreenInfo loadingScreenInfo;
	std::vector<Texture> loadingImage;
	float currentTime;
	float totalTime;
};

template<class T>
inline void SceneManager::LoadScene(LoadingScreenInfo info)
{
	currentTime = 0;
	totalTime = loadingScreenInfo.LoadingImagePath.size() * loadingScreenInfo.LoopTime;

	loadingImage.clear();
	loadingScreenInfo = info;

	loadingThread = std::thread([
		&loadingWindow = loadingWindow, &loadingImage = loadingImage,
			&info = loadingScreenInfo, &eventManager = eventManager,
			&currentScene = currentScene, &sceneInfo = sceneInfo]()
		{
			glfwMakeContextCurrent(loadingWindow);
			std::unique_ptr<Scene> loadingScene = std::make_unique<T>(sceneInfo);

			TextureLoader loader;
			for (const auto& path : info.LoadingImagePath)
			{
				loadingImage.push_back(loader.LoadFromFile(TextureType::Diffuse, path));
			}

			LoadSceneStartEvent startEvt;
			eventManager.SendEvent<LoadSceneStartEvent>(startEvt);

			loadingScene->Setup();

			/* 别问我为什么知道 我也不知道 但是你不写材质就是黑的 keyword:mutlithread glFlush */
			glFinish();
			glFlush();

			currentScene = std::move(loadingScene);

			LoadSceneEndEvent endEvt;
			eventManager.SendEvent<LoadSceneEndEvent>(endEvt);
		});

	loadingThread.detach();
}
