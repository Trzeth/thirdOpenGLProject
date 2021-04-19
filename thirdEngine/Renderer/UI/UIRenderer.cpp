#include "UIRenderer.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

UIRenderer::UIRenderer()
{ }

void UIRenderer::Initialize(GLFWwindow* window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.Fonts->AddFontFromFileTTF("fonts/NotoSansSC-Regular.otf", 20.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsLight();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450 core");
}

UIRenderer::UIElementHandle UIRenderer::GetEntityHandle(const std::shared_ptr<UIElement>& element)
{
	Entity entity;
	entity.element = element;

	UIElementHandle handle = entityPool.GetNewHandle(entity);

	return handle;
}

void UIRenderer::Update(float dt)
{
	for (auto iter = entityPool.begin(); iter != entityPool.end(); iter++) {
		iter->second.element->Update(dt);
	}
}

void UIRenderer::Draw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (auto iter = entityPool.begin(); iter != entityPool.end(); iter++) {
		if (!iter->second.element->IsVisible()) {
			continue;
		}
		iter->second.element->Draw();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}