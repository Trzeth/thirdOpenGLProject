#include "ObjectViewerComponent.h"

ComponentConstructorInfo ObjectViewerComponentConstructor::Construct(World& world, eid_t parent, void* userinfo)
{
	ObjectViewerComponent* component = new ObjectViewerComponent();

	component->materialList = materialList;
	return ComponentConstructorInfo(component, typeid(ObjectViewerComponent).hash_code());
}

void ObjectViewerComponentConstructor::Finish(World& world, eid_t entity)
{
}