#include "ClothStoreUI.h"

#include <thirdEngine/Renderer/TextureImpl.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#include "Game/Event/ClothStoreSceneEvent.h"

ClothStoreUI::ClothStoreUI(EventManager& eventManager)
	:isVisible(true), eventManager(eventManager)
{ }

void ClothStoreUI::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::Begin("Cloth Store Table", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		ImVec2 size(700, 768);
		ImGui::SetWindowSize(size);

		float height = ImGui::GetContentRegionAvail().y / 6;
		float width = ImGui::GetContentRegionAvail().x;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		std::vector<std::string> clothName = { u8"日常衣服",u8"学院校服",u8"西服",u8"休闲服",u8"换好了！" };
		for (int i = 0; i != 5; i++)
		{
			ImGui::Dummy(ImVec2(0, height / 6));

			if (ImGui::Button(clothName[i].c_str(), ImVec2(width, height)))
			{
				ClothStoreChangeClothSceneEvent evt(i);
				eventManager.SendEvent(evt);
			}
		}
		ImGui::PopStyleColor();

		ImGui::SetWindowPos(ImVec2((viewport->Size.x) / 2, (viewport->Size.y - size.y) / 2));

		ImGui::End();
	}
	ImGui::PopFont();
	ImGui::PopStyleVar(2);
}