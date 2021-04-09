#include "CameraSystem.h"

#include  <thirdEngine/Renderer/Renderer.h>

#include "ModelViewer/Component/CameraComponent.h"
#include "ModelViewer/Component/TransformComponent.h"

CameraSystem::CameraSystem(World& world, Renderer& renderer) :System(world), renderer(renderer)
{
	require<CameraComponent>();
	require<TransformComponent>();
}

void CameraSystem::updateEntity(float dt, eid_t entity)
{
	CameraComponent* cameraComponent = world.GetComponent<CameraComponent>(entity);
	TransformComponent* transformComponent = world.GetComponent<TransformComponent>(entity);

	if (!cameraComponent->isEnable)return;

	glm::mat4 matrix = transformComponent->data->Matrix();
	cameraComponent->data.SetInverseViewMatrix(matrix);

	float ratio = 1;

	if (renderer.GetViewportHeight() != 0)
		ratio = (float)renderer.GetViewportWidth() / renderer.GetViewportHeight();

	renderer.SetProjectionMatrix(glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f));
	renderer.SetViewMatrix(cameraComponent->data.GetViewMatrix());
	renderer.SetViewPos(transformComponent->data->GetWorldPosition());
}