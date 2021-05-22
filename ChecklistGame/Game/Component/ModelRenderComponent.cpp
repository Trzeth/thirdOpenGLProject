#include <thirdEngine/Framework/World.h>

#include "ModelRenderComponent.h"
#include "Game/Extra/PrefabConstructionInfo.h"

ComponentConstructorInfo ModelRenderComponentConstructor::Construct(World& world, eid_t parent, void* userinfo)
{
	ModelRenderComponent* component = new ModelRenderComponent();
	component->rendererHandle = renderer.GetRenderableHandle(modelHandle, shader);

	if (userinfo != nullptr) {
		PrefabConstructionInfo* info = (PrefabConstructionInfo*)userinfo;
		renderer.SetRenderableTransform(component->rendererHandle, info->initialTransform.Matrix());
	}

	return ComponentConstructorInfo(component, typeid(ModelRenderComponent).hash_code());
}

void ModelRenderComponentConstructor::Finish(World& world, eid_t entity)
{
	ModelRenderComponent* component = world.GetComponent<ModelRenderComponent>(entity);
	renderer.SetRenderableAnimation(component->rendererHandle, defaultAnimation);
}