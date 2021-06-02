#include "SceneManager.h"

#include <thirdEngine/Renderer/UI/ImGui/imgui.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui_impl_glfw.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui_impl_opengl3.h>

void SceneManager::SetSpawnPosition(const Transform& t)
{
	sceneInfo.spawnPosition = t;
}

void SceneManager::LoadBegin()
{
	loadingImage.clear();

	TextureLoader loader;
	for (const auto& path : loadingScreenInfo.LoadingImagePath)
	{
		loadingImage.push_back(loader.LoadFromFile(TextureType::Diffuse, path));
	}

	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	loadingWindow = glfwCreateWindow(1, 1, "loadingThread", NULL, window.GetWindow());
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	assert(loadingWindow);

	if (currentScene)
	{
		currentScene->PreDestruct();
		currentScene.reset();
	}

	sceneInfo.world->Clear();

	loadingThread = std::thread([
		&loadingWindow = loadingWindow, &eventManager = eventManager,
			&currentScene = currentScene, &currentLoadingScene = currentLoadingScene]()
		{
			LoadSceneStartEvent startEvt;
			eventManager.SendEvent<LoadSceneStartEvent>(startEvt);

			glfwMakeContextCurrent(loadingWindow);

			currentLoadingScene->Setup();

			//std::this_thread::sleep_for(std::chrono::seconds(5000));

			/* 别问我为什么知道 我也不知道 但是你不写材质就是黑的 keyword:mutlithread glFlush */
			glFinish();
			glFlush();

			glfwDestroyWindow(loadingWindow);

			currentScene = std::move(currentLoadingScene);

			LoadSceneEndEvent endEvt;
			eventManager.SendEvent<LoadSceneEndEvent>(endEvt);
		});

	loadingThread.detach();
}

void SceneManager::LoadFinish()
{
	currentScene->Finish();
}

void SceneManager::Update(float dt)
{
	currentTime += dt;

	if (currentTime >= totalTime)
	{
		currentTime -= totalTime;
	}
}

void SceneManager::Draw()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (loadingScreenInfo.LoopTime == 0 || loadingImage.size() == 0)
	{
		return;
	}

	int curPos = currentTime / loadingScreenInfo.LoopTime;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::Begin("Loading", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowSize(ImVec2(400, 583));

		ImGui::SetWindowPos(ImVec2(viewport->Size.x / 2 - (400 / 2), viewport->Size.y / 2 - (583 / 2)));
		ImGui::Image((void*)(intptr_t)loadingImage[curPos].GetID(), ImVec2(400, 583));

		ImGui::End();
	}

	ImGui::PopStyleVar(2);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}