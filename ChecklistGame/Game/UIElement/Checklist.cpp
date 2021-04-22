#include "Checklist.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <thirdEngine/Renderer/UI/ImGui/imgui.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui_internal.h>
#include <thirdEngine/Renderer/TextureImpl.h>

Checklist::Checklist(Input& input, GlobalVariable& globalVariable)
	:input(input), isVisible(true), globalVariable(globalVariable)
{
	TextureLoader textureLoader;
	checklistTexture = textureLoader.LoadFromFile(TextureType::Diffuse, "GUI/Checklist.png");
}

void Checklist::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 140));
	{
		ImGui::Begin("Checklist", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);

		const ImVec2 size(300, 510);

		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		ImVec2 pos = ImGui::GetWindowPos();
		const ImRect image_bb(pos, pos + size);

		drawList->AddImage(ImTextureID(checklistTexture.impl->id), image_bb.Min, image_bb.Max);

		ImGui::Text("%.0f / %f", globalVariable.currentTime, globalVariable.totalTime);

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(221, 0, 27, 255));

		for (int i = 0; i != globalVariable.taskState.size(); i++)
		{
			if (globalVariable.taskState[i] == TaskState::Finished) {
				ImGui::Text("Yes!");
				ImGui::Dummy(ImVec2(0, 10));
			}
			else
			{
				break;
			}
		}

		ImGui::PopStyleColor();

		ImGui::SetWindowSize(ImVec2(300, 510));
		ImGui::SetWindowPos(ImVec2(0, 200));

		ImGui::End();
	}
	ImGui::PopStyleVar(2);
}