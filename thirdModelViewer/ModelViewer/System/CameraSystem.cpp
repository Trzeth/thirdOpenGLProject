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

	glm::mat4 matrix = transformComponent->data->Matrix();
	cameraComponent->data.SetInverseViewMatrix(matrix);

	renderer.SetProjectionMatrix(glm::perspective(glm::radians(45.0f), (float)1920 / 1080, 0.1f, 100.0f));
	renderer.SetViewMatrix(cameraComponent->data.GetViewMatrix());
}