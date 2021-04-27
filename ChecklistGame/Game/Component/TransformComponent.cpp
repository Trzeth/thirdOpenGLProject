#include "TransformComponent.h"

#include <thirdEngine/Framework/World.h>
#include "Game/Extra/Transform.h"
#include "Game/Extra/PrefabConstructionInfo.h"

ComponentConstructorInfo TransformComponentConstructor::Construct(World& world, eid_t parent, void* userinfo)
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