#include "PlayerInputSystem.h"

#include <glm/glm.hpp>
#include <thirdEngine/Input/Input.h>

#include "Game/Component/PlayerComponent.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/RigidbodyMotorComponent.h"

PlayerInputSystem::PlayerInputSystem(World& world, Input& input, EventManager& eventManager) :System(world), input(input), eventManager(eventManager)
{
	require<PlayerComponent>();
	require<TransformComponent>();
}

void PlayerInputSystem::updateEntity(float dt, eid_t entity)
{
	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(entity);
	RigidbodyMotorComponent* rigidbodyMotorComponent = world.GetComponent<RigidbodyMotorComponent>(entity);

	if (playerComponent->controlState != PlayerControlState::Normal) {
		return;
	}

	TransformComponent* cameraTransformComponent = world.GetComponent<TransformComponent>(playerComponent->data.camera);
	TransformComponent* playerTransformComponent = world.GetComponent<TransformComponent>(entity);

	float horizontal = input.GetAxis("Horizontal", Device_Kbm);
	float vertical = input.GetAxis("Vertical", Device_Kbm);
	float lookHorizontal = input.GetAxis("LookHorizontal", Device_Kbm);
	float lookVertical = input.GetAxis("LookVertical", Device_Kbm);

	horizontalRad -= lookHorizontal * dt;
	verticalRad += lookVertical * dt;
	verticalRad = glm::clamp(verticalRad, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);

	float r = 15;
	glm::vec3 cameraPos = playerTransformComponent->data->GetPosition();
	cameraPos.x += cos(horizontalRad) * r;
	cameraPos.y += sin(horizontalRad) * cos(verticalRad) * r + 8;
	cameraPos.z += sin(verticalRad) * sin(horizontalRad) * r + 3;

	cameraTransformComponent->data->SetPosition(cameraPos);
	cameraTransformComponent->data->SetRotation(glm::angleAxis(verticalRad - glm::pi<float>() / 2, Transform::RIGHT) * glm::angleAxis(glm::pi<float>() / 2 - horizontalRad, Transform::UP));

	if (horizontal < 0.0001f && vertical < 0.0001f) {
		playerComponent->SetAnimationState(PlayerAnimationState::Idle);
	}
	else
	{
		playerComponent->SetAnimationState(PlayerAnimationState::Walk);
	}

	if (abs(vertical) > 0.0001f) {
		if (vertical > 0)
			rigidbodyMotorComponent->facing = glm::angleAxis(-glm::atan(horizontal / vertical), Transform::UP);
		else
			rigidbodyMotorComponent->facing = glm::angleAxis(glm::pi<float>() - glm::atan(horizontal / vertical), Transform::UP);
	}
	else if (abs(horizontal) > 0.0001f)
	{
		rigidbodyMotorComponent->facing = glm::angleAxis(-glm::pi<float>() / 2 * horizontal, Transform::UP);
	}

	rigidbodyMotorComponent->movement = glm::vec2(vertical, horizontal);
}