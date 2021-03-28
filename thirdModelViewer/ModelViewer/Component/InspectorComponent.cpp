#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#include "InspectorComponent.h"

#include "ModelViewer/UIElement/Sidebar.h"
#include "ModelViewer/UIElement/DockSpace.h"

ComponentConstructorInfo InspectorComponentConstructor::Construct(World& world, eid_t parent, void* userinfo) const
{
	InspectorComponent* component = new InspectorComponent();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	component->dockSpaceHandle = uiRenderer.GetEntityHandle(std::make_shared<DockSpace>());

	component->sidebar = std::make_shared<Sidebar>();
	component->sidebarHandle = uiRenderer.GetEntityHandle(component->sidebar);

	component->objectViewer = std::make_shared<ObjectViewer>();
	component->objectViewerHandle = uiRenderer.GetEntityHandle(component->objectViewer);

	glGenTextures(1, &component->texColorBuffer);
	glGenRenderbuffers(1, &component->renderBuffer);

	component->frameBuffer = 0;
	// component->texColorBuffer = component->renderBuffer = 0;
	component->preWindowHeight = component->preWindowWidth = 0;

	component->objectViewer->SetTextureID(component->texColorBuffer);

	return ComponentConstructorInfo(component, typeid(InspectorComponent).hash_code());
}

void InspectorComponentConstructor::Finish(World& world, eid_t entity)
{
}