#include "WaterFlower.h"

#include <thirdEngine/Renderer/TextureImpl.h>

#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <thirdEngine/Renderer/UI/ImGui/imgui_internal.h>

#include "Game/Event/YardSceneEvent.h"

WaterFlower::WaterFlower(EventManager& eventManager)
	:isVisible(false), eventManager(eventManager)
{
	TextureLoader textureLoader;
	animationTexture[0] = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Watering1.png");
	animationTexture[1] = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Watering2.png");
	animationTexture[2] = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Watering3.png");
	animationTexture[3] = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Watering4.png");
	animationTexture[4] = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Watering5.png");
	animationTexture[5] = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Watering6.png");

	curTime = 0;
	interTime = 2;
	curPos = 0;
}

void WaterFlower::Update(float dt)
{
	if (!isVisible) return;

	if (curTime > 6 * interTime) {
		isVisible = false;

		YardSceneWaterPotFinishEvent evt;
		eventManager.SendEvent(evt);
	}
	else
	{
		curTime += dt;
		if (curTime >= (curPos + 1) * interTime) {
			if (curPos != 6 - 1)
				curPos++;
		}
	}
}

void WaterFlower::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::Begin("StartMenu", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

		ImGui::SetWindowSize(ImVec2(viewport->Size.x, viewport->Size.y));
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetCursorPos((ImGui::GetWindowSize() - ImVec2(1440, 960)) * 0.5f);

		ImGui::Image((void*)(intptr_t)animationTexture[curPos].GetID(), ImVec2(1440, 960));

		ImGui::End();
	}

	ImGui::PopStyleVar(2);
}

void WaterFlower::Show()
{
	isVisible = true;
}