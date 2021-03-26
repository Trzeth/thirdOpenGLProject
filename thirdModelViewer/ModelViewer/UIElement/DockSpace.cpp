#include "DockSpace.h"

#include <thirdEngine/Renderer/UI/ImGui/imgui.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui_internal.h>

void DockSpace::Draw()
{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	{
		ImGui::Begin("DockSpace Demo", 0, window_flags);

		ImGuiID main = ImGui::GetID("Main");

		ImGui::DockBuilderRemoveNode(main);
		ImGui::DockBuilderAddNode(main, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(main, ImGui::GetMainViewport()->Size);

		ImGuiID dock_main = main;
		ImGuiID dock_right = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Right, 0.3f, nullptr, &dock_main);

		auto node = ImGui::DockBuilderGetNode(dock_main)->Size;

		ImGui::DockBuilderDockWindow("ObjectViewer", dock_main);
		ImGui::DockBuilderDockWindow("Sidebar", dock_right);

		ImGui::DockBuilderFinish(main);

		ImGui::DockSpace(main, ImVec2(0.0f, 0.0f), 0);

		ImGui::End();
	}

	ImGui::PopStyleVar(2);

	ImGui::Begin("Sidebar");
	ImGui::End();

	ImGui::Begin("ObjectViewer");

	ImGui::End();
}