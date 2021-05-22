#include <string>
#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <thirdEngine/Renderer/UI/ImGui/imgui.h>
#include <thirdEngine/Renderer/Material.h>

#include "Sidebar.h"
#include "Lib/ImGuiFileDialog/ImGuiFileDialog.h"

Sidebar::Sidebar(ObjectViewerComponent* objectViewerComponent)
	:objectViewerComponent(objectViewerComponent)
{ }

void Sidebar::Draw()
{
	drawMetricsWindow();
	drawMaterialWindow();
	drawGeneralWindow();
}

void Sidebar::drawMetricsWindow()
{
	ImGui::ShowMetricsWindow();
}

void Sidebar::drawGeneralWindow()
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

void Sidebar::drawMaterialWindow()
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

	drawMaterialProperties(objectViewerComponent->materialList[objectViewerComponent->currentMaterialIndex].material.GetProperties());

	ImGui::End();
}

void Sidebar::drawMaterialProperties(std::map<std::string, MaterialProperty>& properties)
{
	for (auto& keypair : properties) {
		const std::string& name = keypair.first;
		MaterialProperty& property = keypair.second;

		switch (property.type)
		{
		case MaterialPropertyType::Float:
		{
			float val = property.value->flt;
			ImGui::InputFloat(name.c_str(), &val, 0.1f);
			property.value->flt = val;
			break;
		}
		case MaterialPropertyType::Vec3:
		{
			ImGui::DragFloat3(name.c_str(), &property.value->vec3.x, 0.1f);
			break;
		}
		case MaterialPropertyType::Vec4:
		{
			ImGui::DragFloat3(name.c_str(), &property.value->vec4.x, 0.1f);
			break;
		}
		case MaterialPropertyType::Texture:
			if (property.value->texture.GetType() != TextureType::Cubemap)
			{
				if (ImGui::Button(std::string("New Texture:" + name).c_str()))
					ImGuiFileDialog::Instance()->OpenModal("TextureDlg" + name, "Choose File", "Texture file format{.jpg,.png,.tga,.JPG,.PNG,.TGA}", ".");

				if (ImGuiFileDialog::Instance()->Display("TextureDlg" + name))
				{
					// action if OK
					if (ImGuiFileDialog::Instance()->IsOk())
					{
						TextureLoader textureLoader;
						Texture texture = textureLoader.LoadFromFile(property.value->texture.GetType(), ImGuiFileDialog::Instance()->GetFilePathName());
						property.value = std::make_unique<MaterialPropertyValue>(texture);
						// action
					}

					// close
					ImGuiFileDialog::Instance()->Close();
				}
			}
			break;
		default:
			//Invalid Property
			break;
		}
	}
}