#include "CameraSystem.h"

#include  <thirdEngine/Renderer/Renderer.h>

#include "Game/Component/CameraComponent.h"
#include "Game/Component/TransformComponent.h"

CameraSystem::CameraSystem(World& world, Renderer& renderer) :System(world), renderer(renderer)
{
	require<CameraComponent>();
	require<TransformComponent>();
}

void CameraSystem::updateEntity(float dt, eid_t entity)
{
	CameraComponent* cameraComponent = world.GetComponent<CameraComponent>(entity);
	TransformComponent* transformComponent = world.GetComponent<TransformComponent>(entity);

	auto qut = transformComponent->data->GetRotation();

	if (!cameraComponent->isEnable)
		return;

	glm::mat4 matrix = transformComponent->data->Matrix();
	cameraComponent->data.SetInverseViewMatrix(matrix);

	if (renderer.GetViewportHeight() != 0)
		cameraComponent->data.SetScreenSize(renderer.GetViewportWidth(), renderer.GetViewportHeight());

	renderer.SetProjectionMatrix(cameraComponent->data.GetProjectionMatrix());
	renderer.SetViewMatrix(cameraComponent->data.GetViewMatrix());
	renderer.SetViewPos(transformComponent->data->GetWorldPosition());
}