#include "Ending.h"

#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <thirdEngine/Renderer/UI/ImGui/imgui_internal.h>

Ending::Ending()
	:isVisible(true)
{
	TextureLoader textureLoader;
	std::vector<std::string> paths = {
		"GUI/Ending/Succeed/SuccessfulEnding0.png",
		"GUI/Ending/Succeed/SuccessfulEnding1.png",
		"GUI/Ending/Succeed/SuccessfulEnding2.png",
		"GUI/Ending/Succeed/SuccessfulEnding3.png",
		"GUI/Ending/Succeed/SuccessfulEnding4.png"
	};

	for (const auto& path : paths)
	{
		textures.push_back(textureLoader.LoadFromFile(TextureType::Diffuse, path));
	}

	curTime = 0;
	interTime = 2;
	curPos = 0;
}

void Ending::Update(float dt)
{
	if (!isVisible) return;

	if (curTime > textures.size() * interTime) {
		//isVisible = false;
	}
	else
	{
		curTime += dt;
		if (curTime >= (curPos + 1) * interTime) {
			if (curPos != textures.size() - 1)
				curPos++;
		}
	}
}

void Ending::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::Begin("Ending", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

		ImGui::SetWindowSize(ImVec2(viewport->Size.x, viewport->Size.y));
		ImGui::SetWindowPos(ImVec2(0, 0));

		ImVec2 imageSize;
		if (curPos != textures.size() - 1)
			imageSize = ImVec2(819, 1093);
		else
			imageSize = ImVec2(1366, 1024);

		ImGui::SetCursorPos((ImGui::GetWindowSize() - imageSize) * 0.5f);

		ImGui::Image((void*)(intptr_t)textures[curPos].GetID(), imageSize);

		ImGui::End();
	}

	ImGui::PopStyleVar(2);
}