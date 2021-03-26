#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#include "InspectorComponent.h"

#include "ModelViewer/UIElement/Sidebar.h"
#include "ModelViewer/UIElement/DockSpace.h"
#include "ModelViewer/UIElement/ObjectViewer.h"

ComponentConstructorInfo InspectorComponentConstructor::Construct(World& world, eid_t parent, void* userinfo) const
{
	InspectorComponent* component = new InspectorComponent();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	component->sidebarHandle = uiRenderer.GetEntityHandle(std::make_shared<Sidebar>());
	component->objectViewerHandle = uiRenderer.GetEntityHandle(std::make_shared<ObjectViewer>());
	component->dockSpaceHandle = uiRenderer.GetEntityHandle(std::make_shared<DockSpace>());

	return ComponentConstructorInfo(component, typeid(InspectorComponent).hash_code());
}

void InspectorComponentConstructor::Finish(World& world, eid_t entity)
{
}