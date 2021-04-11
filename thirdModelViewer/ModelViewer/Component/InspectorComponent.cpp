#include <thirdEngine/Renderer/UI/ImGui/imgui.h>

#include "InspectorComponent.h"

#include "ModelViewer/UIElement/Sidebar.h"
#include "ModelViewer/UIElement/DockSpace.h"

ComponentConstructorInfo InspectorComponentConstructor::Construct(World& world, eid_t parent, void* userinfo) const
{
	eid_t viewer = *(eid_t*)userinfo;

	InspectorComponent* component = new InspectorComponent();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	component->dockSpaceHandle = uiRenderer.GetEntityHandle(std::make_shared<DockSpace>());

	component->objectViewer = std::make_shared<ObjectViewer>();
	component->objectViewerHandle = uiRenderer.GetEntityHandle(component->objectViewer);

	component->viewer = viewer;

	ObjectViewerComponent* objectViewerComponent = world.GetComponent<ObjectViewerComponent>(component->viewer);

	component->sidebar = std::make_shared<Sidebar>(objectViewerComponent);
	component->sidebarHandle = uiRenderer.GetEntityHandle(component->sidebar);

	glGenTextures(1, &component->texColorBuffer);
	glGenRenderbuffers(1, &component->renderBuffer);
	glGenFramebuffers(1, &component->frameBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, component->frameBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, component->renderBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, component->renderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//component->frameBuffer = 0;
	// component->texColorBuffer = component->renderBuffer = 0;
	component->preWindowHeight = component->preWindowWidth = 0;

	component->objectViewer->SetTextureID(component->texColorBuffer);

	return ComponentConstructorInfo(component, typeid(InspectorComponent).hash_code());
}

void InspectorComponentConstructor::Finish(World& world, eid_t entity)
{ }