#include "TransformComponent.h"

#include <thirdEngine/Framework/World.h>
#include "ModelViewer/Extra/Transform.h"
#include "ModelViewer/Extra/PrefabConstructionInfo.h"

ComponentConstructorInfo TransformComponentConstructor::Construct(World& world, eid_t parent, void* userinfo) const
{
	PrefabConstructionInfo* constructionInfo = (PrefabConstructionInfo*)userinfo;

	TransformComponent* component = new TransformComponent(transform);
	TransformComponent* parentComponent = world.GetComponent<TransformComponent>(parent);

	if (constructionInfo != nullptr) {
		*component->data = constructionInfo->initialTransform;
	}

	if (parent != World::NullEntity) {
		component->data->SetParent(parentComponent->data);
	}

	return ComponentConstructorInfo(component, typeid(TransformComponent).hash_code());
}