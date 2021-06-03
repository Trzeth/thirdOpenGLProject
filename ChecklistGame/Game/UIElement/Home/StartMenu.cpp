#include "StartMenu.h"

#include <thirdEngine/Renderer/TextureImpl.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#include "Game/Extra/ExternalUI.h"
#include "Game/Event/GameEvent.h"

StartMenu::StartMenu(EventManager& eventManager)
	:eventManager(eventManager), isVisible(true)
{
	TextureLoader textureLoader;
	titleTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Title.png");
	playBtnTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/PlayButton.png");
	playBtnHoverTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/PlayButtonHover.png");
	exitBtnTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/QuitButton.png");
	exitBtnHoverTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/QuitButtonHover.png");
}

void StartMenu::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::Begin("StartMenu", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowSize(ImVec2(800, 800));
		ImGui::SetWindowPos(ImVec2(viewport->Size.x / 2 - 400, viewport->Size.y / 2 - 400));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		ImGui::Image((void*)(intptr_t)titleTexture.GetID(), ImVec2(800, 279));

		ImGui::Dummy(ImVec2(0.0f, 150.0f));

		ImGui::Dummy(ImVec2(200.0f, 0.0f));
		ImGui::SameLine();

		if (ImGui::ImageAnimButton((void*)(intptr_t)playBtnTexture.GetID(), (void*)(intptr_t)playBtnHoverTexture.GetID(), ImVec2(400, 121)))
		{
			GameStartEvent evt;
			eventManager.SendEvent(evt);

			isVisible = false;
		}

		ImGui::Dummy(ImVec2(0.0f, 40.0f));

		ImGui::Dummy(ImVec2(200.0f, 40.0f));
		ImGui::SameLine();

		if (ImGui::ImageAnimButton((void*)(intptr_t)exitBtnTexture.GetID(), (void*)(intptr_t)exitBtnHoverTexture.GetID(), ImVec2(400, 121)))
		{
			GameCloseEvent evt;
			eventManager.SendEvent(evt);
			isVisible = false;
		}

		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	ImGui::PopStyleVar(2);
}