#include "ShopUI.h"

#include <thirdEngine/Renderer/TextureImpl.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui.h>
#include "Game/Event/ShopSceneEvent.h"

ShopUI::ShopUI(EventManager& eventManager)
	:isVisible(true), eventManager(eventManager), buttonStatus({ false,false,false,false })
{
	TextureLoader textureLoader;
	npcTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Icon/Cashier.png");
	butterTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Icon/Butter.png");
	cheeseTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Icon/Cheese.png");
	meatTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Icon/Meat.png");
	riceTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Icon/Rice.png");
}

void ShopUI::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::Begin("Shop Deal Table", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		ImVec2 size(1280, 768);
		ImGui::SetWindowSize(size);

		ImGui::Dummy(ImVec2(50, 0));
		ImGui::SameLine();

		ImGui::Image((ImTextureID)npcTexture.GetID(), ImVec2(326, 381));
		ImGui::SameLine();

		ImGui::Dummy(ImVec2(50, 0));
		ImGui::SameLine();

		ImGui::BeginGroup();

		float height = ImGui::GetContentRegionAvail().y / 5;
		float width = 341.0f / 256 * height;
		ImVec2 imageSize = ImVec2(width, height);

		ImGui::Dummy(ImVec2(0, height / 5));
		ImGui::Image((ImTextureID)butterTexture.GetID(), imageSize);
		ImGui::SameLine();

		if (!buttonStatus[0])
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::Button(u8"ÂòÒ»¶äÄÌÓÍ", ImVec2(0, height)))buttonStatus[0] = true;
			ImGui::PopStyleColor(1);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.13, 0.52, 0.09, 1));
			ImGui::Button(u8"ÂôÍêÀ²£¡", ImVec2(0, height));
			ImGui::PopStyleColor(4);
		}

		ImGui::Dummy(ImVec2(0, height / 5));
		ImGui::Image((ImTextureID)cheeseTexture.GetID(), imageSize);
		ImGui::SameLine();

		if (!buttonStatus[1])
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::Button(u8"ÂòÒ»¿éÄÌÀÒ", ImVec2(0, height)))buttonStatus[1] = true;;
			ImGui::PopStyleColor(1);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.13, 0.52, 0.09, 1));
			ImGui::Button(u8"ÂôÍêÀ²£¡", ImVec2(0, height));
			ImGui::PopStyleColor(4);
		}

		ImGui::Dummy(ImVec2(0, height / 5));
		ImGui::Image((ImTextureID)meatTexture.GetID(), imageSize);
		ImGui::SameLine();

		if (!buttonStatus[2])
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::Button(u8"ÂòÒ»¿éÈâ", ImVec2(0, height)))buttonStatus[2] = true;
			ImGui::PopStyleColor(1);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.13, 0.52, 0.09, 1));
			ImGui::Button(u8"ÂôÍêÀ²£¡", ImVec2(0, height));
			ImGui::PopStyleColor(4);
		}

		ImGui::Dummy(ImVec2(0, height / 5));
		ImGui::Image((ImTextureID)riceTexture.GetID(), imageSize);
		ImGui::SameLine();

		if (!buttonStatus[3])
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::Button(u8"ÂòÒ»´ü´óÃ×", ImVec2(0, height)))buttonStatus[3] = true;;
			ImGui::PopStyleColor(1);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.13, 0.52, 0.09, 1));
			ImGui::Button(u8"ÂôÍêÀ²£¡", ImVec2(0, height));
			ImGui::PopStyleColor(4);
		}

		ImGui::EndGroup();

		ImGui::SetWindowPos(ImVec2((viewport->Size.x - size.x) / 2, (viewport->Size.y - size.y) / 2));

		ImGui::End();
	}
	ImGui::PopFont();
	ImGui::PopStyleVar(2);
}