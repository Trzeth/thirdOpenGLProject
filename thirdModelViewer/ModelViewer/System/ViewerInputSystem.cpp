#include <glm/glm.hpp>

#include <thirdEngine/Input/Input.h>

#include "ViewerInputSystem.h"

#include "ModelViewer/Component/CameraComponent.h"
#include "ModelViewer/Component/ViewerComponent.h"
#include "ModelViewer/Component/TransformComponent.h"

ViewerInputSystem::ViewerInputSystem(World& world, Input& input, EventManager& eventManager)
	:System(world), input(input), eventManager(eventManager), horizontalRad(0), verticalRad(0)
{
	require<ViewerComponent>();
}

void ViewerInputSystem::updateEntity(float dt, eid_t entity)
{
	ViewerComponent* viewerComponent = world.GetComponent<ViewerComponent>(entity);

	float horizontal = input.GetAxis("Horizontal", Device_Kbm);
	float vertical = input.GetAxis("Vertical", Device_Kbm);
	float lookHorizontal = input.GetAxis("LookHorizontal", Device_Kbm);
	float lookVertical = input.GetAxis("LookVertical", Device_Kbm);

	horizontalRad -= lookHorizontal * dt;
	verticalRad += lookVertical * dt;
	verticalRad = glm::clamp(verticalRad, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);

	CameraComponent* fixedCamera = world.GetComponent<CameraComponent>(viewerComponent->data.FixedCamera);
	CameraComponent* fpsCamera = world.GetComponent<CameraComponent>(viewerComponent->data.FPSCamera);

	if (viewerComponent->viewerState == ViewerState::FPSCamera) {
		TransformComponent* cameraTransform = world.GetComponent<TransformComponent>(viewerComponent->data.FixedCamera);
		fixedCamera->isEnable = false;
		fpsCamera->isEnable = true;

		cameraTransform->data->SetRotation(glm::angleAxis(verticalRad, Transform::RIGHT));
		cameraTransform->data->SetRotation(glm::angleAxis(horizontalRad, Transform::UP));
	}
	else // Fixed Camera
	{
		TransformComponent* cameraTransform = world.GetComponent<TransformComponent>(viewerComponent->data.FixedCamera);
	}
}