#include "DeerNPCConversation.h"
#include <string>

#include <thirdEngine/Renderer/UI/ImGui/imgui.h>
#include "Game/Event/ForestSceneEvent.h"

DeerNPCConversation::DeerNPCConversation(EventManager& eventManager)
	: isVisible(false), eventManager(eventManager), index(0)
{
	std::vector<std::string> paths =
	{
		"GUI/ForestConversation/0.png",
		"GUI/ForestConversation/1.png",
		"GUI/ForestConversation/2.png",
		"GUI/ForestConversation/3.png",
		"GUI/ForestConversation/4.png",
		"GUI/ForestConversation/5.png",
		"GUI/ForestConversation/6.png"
	};
	TextureLoader textureLoader;

	for (const auto path : paths)
		textures.push_back(textureLoader.LoadFromFile(TextureType::Diffuse, path));
}

void DeerNPCConversation::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::Begin("StartMenu", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowSize(ImVec2(1087, 336));
		ImGui::SetWindowPos(ImVec2((viewport->Size.x - 1087) / 2, viewport->Size.y / 2));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton((void*)(intptr_t)textures[index].GetID(), ImVec2(1087, 336))) {
			index++;
			if (index == 6)
			{
				isVisible = false;
				ForestSceneNPCInteractEvent evt;
				eventManager.SendEvent<ForestSceneNPCInteractEvent>(evt);
			}
			else if (index == 7)
			{
				ForestSceneGoHomeInteractEvent evt;
				eventManager.SendEvent<ForestSceneGoHomeInteractEvent>(evt);
			}
		}

		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	ImGui::PopStyleVar(2);
}

void DeerNPCConversation::Show()
{
	isVisible = true;
}