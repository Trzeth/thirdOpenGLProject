#include "ItemInfo.h"
#include <thirdEngine/Renderer/UI/ImGui/imgui.h>
#include "Game/Event/HouseSceneEvent.h"

ItemInfo::ItemInfo()
	: isVisible(false), index(0)
{
	TextureLoader textureLoader;

	textures.push_back(textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Gift.png"));
	textures.push_back(textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Songbing.png"));
	textures.push_back(textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Bangtang.png"));
}

void ItemInfo::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::Begin("Item Info", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);

		ImVec2 size = ImVec2(1280, 768);
		ImGui::SetWindowSize(size);
		ImGui::SetWindowPos(ImVec2((viewport->Size.x - size.x) / 2, (viewport->Size.y - size.y) / 2));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton(ImTextureID(textures[index].GetID()), size)) {
			isVisible = false;
		}

		ImGui::PopStyleColor(3);

		ImGui::End();
	}

	ImGui::PopStyleVar(2);
}

void ItemInfo::Show(unsigned index)
{
	this->index = index;
	isVisible = true;
}