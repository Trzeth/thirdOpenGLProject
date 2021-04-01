#include <string>
#include <Windows.h>
#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#include "Sidebar.h"
#include "Lib/ImGuiFileDialog/ImGuiFileDialog.h"

Sidebar::Sidebar(ObjectViewerComponent* objectViewerComponent)
	:objectViewerComponent(objectViewerComponent)
{ }

void Sidebar::Draw()
{
	DrawMetricsWindow();
	DrawMaterialWindow();
	DrawGeneralWindow();
}

void Sidebar::DrawMetricsWindow()
{
	ImGui::ShowMetricsWindow();
}

void Sidebar::DrawGeneralWindow()
{
	//File Info
	ImGui::Begin("General");

	if (ImGui::Button("Open New Model"))
		ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose File", "Model file format{.fbx,.obj,.dae,.FBX,.OBJ,.DAE}", ".");

	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			if (objectViewerComponent->fileInfo.FullPath != ImGuiFileDialog::Instance()->GetFilePathName()) {
				ModelFileInfo fileInfo;
				fileInfo.FullPath = ImGuiFileDialog::Instance()->GetFilePathName();
				fileInfo.FileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
				fileInfo.DirPath = ImGuiFileDialog::Instance()->GetCurrentPath();
				objectViewerComponent->fileInfo = fileInfo;
				objectViewerComponent->fileChangedFlag = true;
			}

			// action
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}

	auto fileInfo = objectViewerComponent->fileInfo;
	ImGui::Text("File Name:%s", fileInfo.FileName.c_str());
	ImGui::Text("Working Dir:%s", fileInfo.DirPath.c_str());

	//Maybe more info?

	{
		//Camera
		{
			const char* cameraType[] = { "Flycam", "Arcball" };

			ImGui::Text("Control");
			ImGui::SameLine();

			int cur = 1;
			ImGui::Combo("##CameraControl", &cur, cameraType, IM_ARRAYSIZE(cameraType));
		}

		ImGui::Button("Reset Camera");
	}

	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen)) {
		//Multi Select of animation
		ImGui::Text("Animations Name");

		const auto& animationNameList = objectViewerComponent->fileInfo.AnimationNameList;
		for (int i = 0; i != animationNameList.size(); i++) {
			if (ImGui::Selectable(animationNameList[i].c_str(), objectViewerComponent->currentAnimationIndex == i)) {
				objectViewerComponent->currentAnimationIndex = i;
				objectViewerComponent->animationChangedFlag = true;
			}
		}

		ImGui::Button("Play");

		ImGui::SameLine();

		ImGui::Button("Pause");
	}

	ImGui::End();
}

void Sidebar::DrawMaterialWindow()
{
	ImGui::Begin("Material");

	const auto& materialList = objectViewerComponent->materialList;

	int index = 0;
	for (auto it = materialList.begin(); it != materialList.end(); it++, index++) {
		if (ImGui::Selectable(it->name.c_str(), index == objectViewerComponent->currentMaterialIndex)) {
			objectViewerComponent->currentMaterialIndex = index;
			objectViewerComponent->materialChangedFlag = true;
		}
	}

	if (ImGui::Button("Reload Current Shader")) {
		objectViewerComponent->materialReloadFlag = true;
	}

	ImGui::End();
}