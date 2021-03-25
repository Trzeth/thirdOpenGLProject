#include <glm/glm.hpp>

#include <thirdEngine/Input/Input.h>

#include "ViewerInputSystem.h"

#include "ModelViewer/Component/CameraComponent.h"
#include "ModelViewer/Component/ObjectViewerComponent.h"
#include "ModelViewer/Component/TransformComponent.h"

ViewerInputSystem::ViewerInputSystem(World& world, Input& input, EventManager& eventManager)
	:System(world), input(input), eventManager(eventManager), horizontalRad(0.0f), verticalRad(0.0f)
{
	require<ObjectViewerComponent>();
}

void ViewerInputSystem::updateEntity(float dt, eid_t entity)
{
	ObjectViewerComponent* viewerComponent = world.GetComponent<ObjectViewerComponent>(entity);

	float horizontal = input.GetAxis("Horizontal", Device_Kbm) * CAMERASPEED;
	float vertical = input.GetAxis("Vertical", Device_Kbm) * CAMERASPEED;
	float lookHorizontal = input.GetAxis("LookHorizontal", Device_Kbm);
	float lookVertical = input.GetAxis("LookVertical", Device_Kbm);

	horizontalRad -= lookHorizontal * dt;
	verticalRad -= lookVertical * dt;
	verticalRad = glm::clamp(verticalRad, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);

	CameraComponent* fixedCamera = world.GetComponent<CameraComponent>(viewerComponent->data.FixedCamera);
	CameraComponent* fpsCamera = world.GetComponent<CameraComponent>(viewerComponent->data.FPSCamera);

	if (viewerComponent->viewerState == ViewerState::FPSCamera) {
		fixedCamera->isEnable = false;
		fpsCamera->isEnable = true;
		Transform& transform = *world.GetComponent<TransformComponent>(viewerComponent->data.FPSCamera)->data;

		glm::quat rot = glm::vec3(verticalRad, horizontalRad, 0.0f);
		transform.SetRotation(rot);

		glm::vec3 newPos = transform.GetPosition() + transform.GetForward() * vertical + glm::cross(transform.GetForward(), Transform::UP) * horizontal;
		transform.SetPosition(newPos);
	}
	else // Fixed Camera
	{
		TransformComponent* cameraTransform = world.GetComponent<TransformComponent>(viewerComponent->data.FixedCamera);
	}
}