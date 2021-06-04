#pragma once
#include <memory>
#include <vector>
#include <string>
#include <thread>

#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Renderer/Texture.h>
#include <thirdEngine/Renderer/RenderUtil.h>

#include "Game/Event/LoadSceneEvent.h"
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
		:LoadingImagePath(), LoopTime(), EnterEffect(), ExitEffect(), ImageWidth(), ImageHeight()
	{ }

	LoadingScreenInfo(std::vector<std::string> loadingImagePath, float ImageWidth, float ImageHeight, float loopTime, Effect enter = Effect::None, Effect exit = Effect::None)
		:LoadingImagePath(loadingImagePath), LoopTime(loopTime), EnterEffect(enter), ExitEffect(exit), ImageHeight(ImageHeight), ImageWidth(ImageWidth)
	{ }

	std::vector<std::string> LoadingImagePath;

	float ImageWidth;
	float ImageHeight;

	float LoopTime;

	Effect EnterEffect;
	Effect ExitEffect;
};

class SceneManager
{
public:
	SceneManager(Window& window, const SceneInfo& sceneInfo)
		:sceneInfo(sceneInfo), eventManager(*sceneInfo.eventManager),
		window(window), loadingWindow(nullptr)
	{
		//Don't forget to set it
		this->sceneInfo.sceneManager = this;
	}

	template<class T>
	void LoadScene(LoadingScreenInfo info);

	void SetSpawnPosition(const Transform& t);

	void LoadBegin();

	// On Main thread
	void LoadFinish();

	void Update(float dt);

	/* Draw Loading Screen */
	void Draw();
private:
	SceneInfo sceneInfo;
	EventManager& eventManager;
	Window& window;

	std::unique_ptr<Scene> currentScene;
	std::unique_ptr<Scene> currentLoadingScene;

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
	loadingScreenInfo = info;

	currentTime = 0;
	totalTime = loadingScreenInfo.LoadingImagePath.size() * loadingScreenInfo.LoopTime;

	currentLoadingScene = std::make_unique<T>(sceneInfo);

	LoadSceneWaitEvent evt;
	eventManager.SendEvent<LoadSceneWaitEvent>(evt);
}
