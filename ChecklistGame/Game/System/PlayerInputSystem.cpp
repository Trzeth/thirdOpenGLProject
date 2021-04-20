#include "PlayerInputSystem.h"

#include <glm/glm.hpp>
#include <thirdEngine/Input/Input.h>
#include "Game/Component/PlayerComponent.h"
#include "Game/Component/TransformComponent.h"

PlayerInputSystem::PlayerInputSystem(World& world, Input& input, EventManager& eventManager) :System(world), input(input), eventManager(eventManager)
{
	require<PlayerComponent>();
	require<TransformComponent>();
}

void PlayerInputSystem::updateEntity(float dt, eid_t entity)
{
	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(entity);

	if (playerComponent->controlState != PlayerControlState::Normal) {
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

	glm::vec3 cameraPos = cameraTransformComponent->data->GetPosition();
	float r = 10;

	cameraPos.x = cos(horizontalRad) * r;
	cameraPos.y = sin(horizontalRad) * cos(verticalRad) * r + 5;
	cameraPos.z = sin(verticalRad) * sin(horizontalRad) * r;

	cameraTransformComponent->data->SetPosition(cameraPos);
	cameraTransformComponent->data->SetRotation(glm::angleAxis(verticalRad - glm::pi<float>() / 2, Transform::RIGHT) * glm::angleAxis(glm::pi<float>() / 2 - horizontalRad, Transform::UP));

	TransformComponent* transformComponent = world.GetComponent<TransformComponent>(entity);
	glm::vec3 pos = transformComponent->data->GetPosition();
	pos += glm::vec3(horizontal, 0, -vertical);
	transformComponent->data->SetPosition(pos);

	if (horizontal < 0.0001f && vertical < 0.0001f) {
		playerComponent->SetAnimationState(PlayerAnimationState::Idle);
	}
	else
	{
		playerComponent->SetAnimationState(PlayerAnimationState::Walk);
	}
}