#include "Letter.h"

#include <thirdEngine/Renderer/TextureImpl.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui.h>
#include "Game/Event/YardSceneEvent.h"

Letter::Letter(EventManager& eventManager)
	:isVisible(false), eventManager(eventManager)
{
	TextureLoader textureLoader;
	letterTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Letter.png");
}

void Letter::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::Begin("StartMenu", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowSize(ImVec2(1280, 768));
		ImGui::SetWindowPos(ImVec2(viewport->Size.x / 2 - 640, viewport->Size.y / 2 - 384));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton((void*)(intptr_t)letterTexture.GetID(), ImVec2(1280, 768))) {
			isVisible = false;

			YardSceneLetterCloseEvent evt;
			eventManager.SendEvent<YardSceneLetterCloseEvent>(evt);
		}

		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	ImGui::PopStyleVar(2);
}

void Letter::Show()
{
	isVisible = true;
}