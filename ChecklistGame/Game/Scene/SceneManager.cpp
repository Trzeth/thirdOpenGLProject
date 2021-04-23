#include "SceneManager.h"

#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

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
	int n = totalTime == 0 ? 1 : currentTime / totalTime;

	/*
	ImGui::Begin("Loading");
	ImGui::Text("%d", n);
	ImGui::End();
	*/
}