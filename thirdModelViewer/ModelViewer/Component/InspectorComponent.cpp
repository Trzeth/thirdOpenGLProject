#include "InspectorComponent.h"
#include "ModelViewer/UIElement/Sidebar.h"

ComponentConstructorInfo InspectorComponentConstructor::Construct(World& world, eid_t parent, void* userinfo) const
{
	InspectorComponent* component = new InspectorComponent();
	component->uiElementHandle = uiRenderer.GetEntityHandle(std::make_shared<Sidebar>());

	return ComponentConstructorInfo(component, typeid(InspectorComponent).hash_code());
}

void InspectorComponentConstructor::Finish(World& world, eid_t entity)
{
}