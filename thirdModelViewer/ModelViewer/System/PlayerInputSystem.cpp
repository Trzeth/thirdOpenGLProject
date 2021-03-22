#include <glm/glm.hpp>

#include "PlayerInputSystem.h"

#include "ModelViewer/Component/PlayerComponent.h"
#include "ModelViewer/Component/TransformComponent.h"
#include "ModelViewer/Component/RigidbodyMotorComponent.h"

PlayerInputSystem::PlayerInputSystem(World& world, Input& input, EventManager& eventManager) :System(world), input(input), eventManager(eventManager)
{
	require<RigidbodyMotorComponent>();
	require<PlayerComponent>();
}

void PlayerInputSystem::updateEntity(float dt, eid_t entity)
{
	RigidbodyMotorComponent* rigidbodyMotorComponent = world.GetComponent<RigidbodyMotorComponent>(entity);
	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(entity);

	if (playerComponent->isBusy) {
		return;
	}

	TransformComponent* cameraTransformComponent = world.GetComponent<TransformComponent>(playerComponent->data.camera);

	float horizontal = input.GetAxis("Horizontal", Device_Kbm);
	float vertical = input.GetAxis("Vertical", Device_Kbm);
	float lookHorizontal = input.GetAxis("LookHorizontal", Device_Kbm);
	float lookVertical = input.GetAxis("LookVertical", Device_Kbm);

	horizontalRad -= lookHorizontal * dt;
	verticalRad += lookVertical * dt;
	verticalRad = glm::clamp(verticalRad, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);

	cameraTransformComponent->data->SetRotation(glm::angleAxis(verticalRad, Transform::RIGHT));

	rigidbodyMotorComponent->facing = glm::quat(glm::vec3(verticalRad, horizontalRad, 0.0f));
	rigidbodyMotorComponent->movement = glm::vec2(vertical, horizontal);
}