#include "ModelRenderSystem.h"

#include "Game/Component/ModelRenderComponent.h"
#include "Game/Component/TransformComponent.h"

#include <thirdEngine/Renderer/Renderer.h>

ModelRenderSystem::ModelRenderSystem(World& world, Renderer& renderer) :System(world), renderer(renderer)
{
	require<ModelRenderComponent>();
	require<TransformComponent>();
}

void ModelRenderSystem::updateEntity(float dt, eid_t entity)
{
	ModelRenderComponent* modelComponent = world.GetComponent<ModelRenderComponent>(entity);
	TransformComponent* transformComponent = world.GetComponent<TransformComponent>(entity);

	renderer.SetRenderableTransform(modelComponent->rendererHandle, transformComponent->data->Matrix());
}