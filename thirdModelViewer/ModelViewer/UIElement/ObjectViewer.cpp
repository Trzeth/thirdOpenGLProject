#include "ObjectViewer.h"

#include <glad/glad.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

void ObjectViewer::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("ObjectViewer", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
	{
		windowHeight = ImGui::GetContentRegionAvail().y;
		windowWidth = ImGui::GetContentRegionAvail().x;

		ImGui::Image((ImTextureID)textureID, ImVec2(windowWidth, windowHeight), ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();

	ImGui::PopStyleVar();
}

void ObjectViewer::SetTextureID(unsigned int id)
{
	textureID = id;
}

float ObjectViewer::GetWindowWidth()
{
	return windowWidth;
}

float ObjectViewer::GetWindowHeight()
{
	return windowHeight;
}