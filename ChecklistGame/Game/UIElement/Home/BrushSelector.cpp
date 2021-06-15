#include "BrushSelector.h"

#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <thirdEngine/Renderer/UI/ImGui/imgui_internal.h>

#include "Game/Extra/ExternalUI.h"
#include "Game/Event/YardSceneEvent.h"

BrushSelector::BrushSelector(EventManager& eventManager)
	:isVisible(false), eventManager(eventManager)
{
	TextureLoader textureLoader;
	brushTexture[0] = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/BrushSelector1.png");
	brushTexture[1] = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/BrushSelector2.png");
	brushTexture[2] = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/BrushSelector3.png");
	brushTexture[3] = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/BrushSelector4.png");
	closeBtnTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/BrushSelectorButton.png");
	closeBtnHoverTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/BrushSelectorButtonHover.png");
}

void BrushSelector::Draw()
{
	bool hasChanged = false;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	{
		ImGui::Begin("StartMenu", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);

		const ImVec2 size(800, 311);

		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		const ImRect image_bb(window->DC.CursorPos, window->DC.CursorPos + size);

		switch (curBrushSelected)
		{
		case 0:
			drawList->AddImage(ImTextureID(brushTexture[0].GetID()), image_bb.Min, image_bb.Max);
			break;
		case 1:
			drawList->AddImage(ImTextureID(brushTexture[1].GetID()), image_bb.Min, image_bb.Max);
			break;
		case 2:
			drawList->AddImage(ImTextureID(brushTexture[2].GetID()), image_bb.Min, image_bb.Max);
			break;
		case 3:
			drawList->AddImage(ImTextureID(brushTexture[3].GetID()), image_bb.Min, image_bb.Max);
			break;
		default:
			break;
		}

		ImGui::SetWindowSize(ImVec2(800, 800));

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetWindowPos(ImVec2(viewport->Size.x / 2 - 400, viewport->Size.y / 4 * 3 - 100));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		ImGui::Dummy(ImVec2(720, 0));
		ImGui::SameLine();

		if (ImGui::ImageAnimButton((void*)(intptr_t)closeBtnTexture.GetID(), (void*)(intptr_t)closeBtnHoverTexture.GetID(), ImVec2(50, 50)))
		{
			isVisible = false;
			YardSceneBrushFinishEvent evt;
			eventManager.SendEvent(evt);
		}

		ImGui::Dummy(ImVec2(30, 0));
		ImGui::SameLine();
		ImGui::PushID("1");
		if (ImGui::Button("", ImVec2(150, 150)))
		{
			hasChanged = true;
			curBrushSelected = 0;
		}
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::Dummy(ImVec2(20, 0));
		ImGui::SameLine();
		ImGui::PushID("2");
		if (ImGui::Button("", ImVec2(150, 150)))
		{
			hasChanged = true;
			curBrushSelected = 1;
		}
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::Dummy(ImVec2(20, 0));
		ImGui::SameLine();
		ImGui::PushID("3");
		if (ImGui::Button("", ImVec2(150, 150)))
		{
			hasChanged = true;
			curBrushSelected = 2;
		}
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::Dummy(ImVec2(17, 0));
		ImGui::SameLine();
		ImGui::PushID("4");
		if (ImGui::Button("", ImVec2(150, 150)))
		{
			hasChanged = true;
			curBrushSelected = 3;
		}
		ImGui::PopID();

		ImGui::PopStyleColor(3);

		ImGui::End();
	}

	ImGui::PopStyleVar(2);

	if (hasChanged)
	{
		YardSceneBrushChangedEvent evt;
		eventManager.SendEvent(evt);
	}
}

void BrushSelector::Show()
{
	isVisible = true;
}

unsigned int BrushSelector::GetCurrentSelected() const
{
	return curBrushSelected;
}