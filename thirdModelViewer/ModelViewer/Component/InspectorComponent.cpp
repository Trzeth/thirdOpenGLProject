#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#include "InspectorComponent.h"

#include "ModelViewer/UIElement/Sidebar.h"
#include "ModelViewer/UIElement/DockSpace.h"

ComponentConstructorInfo InspectorComponentConstructor::Construct(World& world, eid_t parent, void* userinfo) const
{
	InspectorComponentConstructorInfo* info = (InspectorComponentConstructorInfo*)userinfo;

	InspectorComponent* component = new InspectorComponent();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	component->dockSpaceHandle = uiRenderer.GetEntityHandle(std::make_shared<DockSpace>());

	component->objectViewer = std::make_shared<ObjectViewer>();
	component->objectViewerHandle = uiRenderer.GetEntityHandle(component->objectViewer);

	component->viewer = info->viewer;

	ObjectViewerComponent* objectViewerComponent = world.GetComponent<ObjectViewerComponent>(component->viewer);

	std::vector<std::pair<ShaderFileInfo, Shader>> shaderList;
	for (const auto& shaderFileInfo : info->shaderFileInfos) {
		shaderList.push_back(std::pair<ShaderFileInfo, Shader>(shaderFileInfo, shaderLoader.CompileAndLink(shaderFileInfo.VertexShaderPath, shaderFileInfo.FragmentShaderPath)));
	}

	objectViewerComponent->shaderList = shaderList;

	component->sidebar = std::make_shared<Sidebar>(objectViewerComponent);
	component->sidebarHandle = uiRenderer.GetEntityHandle(component->sidebar);

	glGenTextures(1, &component->texColorBuffer);
	glGenRenderbuffers(1, &component->renderBuffer);

	component->frameBuffer = 0;
	// component->texColorBuffer = component->renderBuffer = 0;
	component->preWindowHeight = component->preWindowWidth = 0;

	component->objectViewer->SetTextureID(component->texColorBuffer);

	return ComponentConstructorInfo(component, typeid(InspectorComponent).hash_code());
}

void InspectorComponentConstructor::Finish(World& world, eid_t entity)
{ }