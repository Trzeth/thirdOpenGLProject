#include <thirdEngine/Renderer/UI/ImGui/imgui.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui_impl_glfw.h>

#include "Sidebar.h"

void Sidebar::Draw()
{
	ImGui::ShowDemoWindow();

	/*
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	{
		ImGui::Begin("Sidebar", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

		const ImGuiIO& io = ImGui::GetIO();

		ImVec2 displaySize = io.DisplaySize;

		float width = ImGui::GetWindowWidth() >= 400 ? ImGui::GetWindowWidth() : 400;

		ImVec2 size(width, displaySize.y);

		ImGui::SetWindowSize(size);
		ImGui::SetWindowPos(ImVec2(displaySize.x - width, 0));

		ImGui::End();
	}
	ImGui::PopStyleVar(2);
	*/
}