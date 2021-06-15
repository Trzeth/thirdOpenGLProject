#include "BikeMap.h"

#include <thirdEngine/Renderer/UI/ImGui/imgui.h>
#include "Game/Event/LoadSceneEvent.h"

BikeMap::BikeMap(EventManager& eventManager, unsigned type)
	:eventManager(eventManager), type(type)
{
	if (type == 1)isVisible = false;
	else isVisible = true;

	TextureLoader textureLoader;
	mapTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Map/0.png");
}

void BikeMap::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::Begin("Bike Map", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);

		if (type == 0)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0, 1.0, 1.0, 1.0));

			ImVec2 size = ImVec2(144, 55);
			ImGui::SetWindowSize(size);
			ImGui::SetWindowPos(ImVec2((viewport->Size.x - size.x), 100));
			if (ImGui::Button(u8"µØÍ¼", size))
			{
				type = 1;
			}
			ImGui::PopStyleColor(1);
		}
		else
		{
			ImVec2 size = ImVec2(1642, 942);
			ImGui::SetWindowSize(size);
			ImGui::SetWindowPos(ImVec2((viewport->Size.x - size.x) / 2, (viewport->Size.y - size.y) / 2));

			auto i = ImGui::GetBackgroundDrawList();
			i->AddImage((ImTextureID)mapTexture.GetID(),
				ImVec2((viewport->Size.x - size.x) / 2, (viewport->Size.y - size.y) / 2),
				ImVec2((viewport->Size.x + size.x) / 2, (viewport->Size.y + size.y) / 2));

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

			for (int i = 0; i != 3; i++)
			{
				ImGui::PushID((std::string("BikeMapButon") + std::to_string(i)).c_str());
				if (ImGui::Button("", ImVec2(size.x / 3, 942))) {
					BikeMapChangeMapEvent evt;
					evt.dst = i;
					eventManager.SendEvent<BikeMapChangeMapEvent>(evt);
				}
				ImGui::PopID();

				ImGui::SameLine();
			}
			ImGui::PopStyleColor(3);
		}

		ImGui::End();
	}

	ImGui::PopStyleVar(2);
}

void BikeMap::Show()
{
	isVisible = true;
}