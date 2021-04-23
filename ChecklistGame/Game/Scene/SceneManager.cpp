#include "SceneManager.h"

#include <thirdEngine/Renderer/UI/ImGui/imgui.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui_impl_glfw.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui_impl_opengl3.h>

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

	if (loadingScreenInfo.LoopTime == 0)
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
		ImGui::Image((void*)(intptr_t)loadingImage[curPos].impl->id, ImVec2(400, 583));

		ImGui::End();
	}

	ImGui::PopStyleVar(2);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}