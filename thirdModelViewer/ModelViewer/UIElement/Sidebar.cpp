#include <string>
#include <Windows.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#include "Sidebar.h"
#include "Lib/ImGuiFileDialog/ImGuiFileDialog.h"

Sidebar::Sidebar()
	:curCameraType(0), cameraSpeed(0.0f), resetCamera(false), playAnim(false), pauseAnim(false)
	, modelPath(), path(), fileName()
	, modelChangedFlag(false), animNameChangedFlag(false)
	, animNames(), curAnimName()
{
}

void Sidebar::Draw()
{
	//Metrics Window
	ImGui::ShowMetricsWindow();

	//Material Editor

	ImGui::Begin("Material");

	ImGui::Text("Texture");

	for (int i = 0; i != animNames.size(); i++) {
		if (ImGui::Selectable(animNames[i].c_str(), curAnimName == animNames[i])) {
			curAnimName = animNames[i];
			animNameChangedFlag = true;
		}
	}

	ImGui::End();

	//File Info
	ImGui::Begin("General");

	if (ImGui::Button("Open New Model"))
		ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose File", "Model file format{.fbx,.obj,.dae,.FBX,.OBJ,.DAE}", ".");

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

	ImGui::Text("File Name:%s", fileName.c_str());

	ImGui::Text("Working Dir:%s", path.c_str());

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

	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen)) {
		//Multi Select of animation
		ImGui::Text("Animations Name");

		for (int i = 0; i != animNames.size(); i++) {
			if (ImGui::Selectable(animNames[i].c_str(), curAnimName == animNames[i])) {
				curAnimName = animNames[i];
				animNameChangedFlag = true;
			}
		}

		if (ImGui::Button("Play"))playAnim = true;

		ImGui::SameLine();

		if (ImGui::Button("Pause"))pauseAnim = false;
	}

	ImGui::End();
}