#include <string>
#include <Windows.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#include "Sidebar.h"
#include "Lib/ImGuiFileDialog/ImGuiFileDialog.h"

Sidebar::Sidebar()
	:curCameraType(0), cameraSpeed(0.0f), resetCamera(false), playAnim(false), pauseAnim(false), modelPath(), path(), fileName(), modelChangedFlag(false)
{
}

void Sidebar::Draw()
{
	//Metrics Window
	ImGui::ShowMetricsWindow();

	//Material Editor

	ImGui::Begin("Material");

	ImGui::End();

	//File Info
	ImGui::Begin("General");

	if (ImGui::Button("Open New Model"))
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", "{.fbx,.obj,.dae,.FBX,.OBJ,.DAE}", ".");

	// display
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			if (modelPath != ImGuiFileDialog::Instance()->GetFilePathName()) {
				modelPath = ImGuiFileDialog::Instance()->GetFilePathName();
				fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
				path = ImGuiFileDialog::Instance()->GetCurrentPath();
				modelChangedFlag = true;
			}

			// action
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::Text("Filename:%s", fileName.c_str());

	ImGui::Text("Working dir:%s", path.c_str());

	//Maybe more info?

	{
		//Camera

		{
			const char* cameraType[] = { "Flycam", "Arcball" };

			ImGui::Text("Control");
			ImGui::SameLine();
			ImGui::Combo("##CameraControl", &curCameraType, cameraType, IM_ARRAYSIZE(cameraType));
		}

		ImGui::Button("Reset Camera");
	}

	if (ImGui::CollapsingHeader("Animation")) {
		//Multi Select of animation

		ImGui::Text("Anima Name");

		if (ImGui::Button("Play"))playAnim = true;

		ImGui::SameLine();

		if (ImGui::Button("Pause"))pauseAnim = false;
	}

	ImGui::End();
}