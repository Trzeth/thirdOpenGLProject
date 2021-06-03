#include "BrushLoading.h"

#include <thirdEngine/Renderer/TextureImpl.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <thirdEngine/Renderer/UI/ImGui/imgui_internal.h>

#include "Game/Event/YardSceneEvent.h"

BrushLoading::BrushLoading(EventManager& eventManager)
	:isVisible(false), eventManager(eventManager)
{
	// Re Pos in Show()
	curXPos = 0;
	curTime = 0;

	hasSend = false;
	TextureLoader textureLoader;
	brushLoadingTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/BrushLoading.png");
}

void BrushLoading::Update(float dt)
{
	if (!isVisible) return;

	if (curTime > 4.0) {
		isVisible = false;
	}
	else
	{
		curTime += dt;
		curXPos = (-2010 - 2010) / 4.0 * curTime + 2010;
	}

	if (curTime > 2.0 && !hasSend)
	{
		YardSceneBrushLoadingMidEvent evt;
		eventManager.SendEvent(evt);
		hasSend = true;
	}
}

void BrushLoading::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::Begin("Loading", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowSize(ImVec2(viewport->Size.x, viewport->Size.y));
		ImGui::SetWindowPos(ImVec2(0, 0));

		ImGui::SetCursorPos((ImVec2(curXPos, (ImGui::GetWindowSize().y - 1080) * 0.5f)));

		ImGui::Image((void*)(intptr_t)brushLoadingTexture.GetID(), ImVec2(2081, 1080));

		ImGui::End();
	}

	ImGui::PopStyleVar(2);
}

void BrushLoading::Show()
{
	isVisible = true;
	hasSend = false;
	curXPos = 2010;
	curTime = 0;
}