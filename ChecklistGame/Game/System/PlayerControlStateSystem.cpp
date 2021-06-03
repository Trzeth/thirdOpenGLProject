#include "PlayerControlStateSystem.h"

#include "Game/Window.h"
#include "Game/Component/PlayerComponent.h"
#include <Game/Component/RigidbodyMotorComponent.h>

PlayerControlStateSystem::PlayerControlStateSystem(World& world, Window& window)
	:System(world), window(window)
{
	require<PlayerComponent>();
}

void PlayerControlStateSystem::updateEntity(float dt, eid_t entity)
{
	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(entity);
	RigidbodyMotorComponent* rigidbodyMotorComponent = world.GetComponent<RigidbodyMotorComponent>(entity);

	if (playerComponent->preControlState != playerComponent->controlState)
	{
		if (playerComponent->controlState == PlayerControlState::Normal
			&& playerComponent->preControlState == PlayerControlState::InGUI)
		{
			//glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		if (playerComponent->controlState == PlayerControlState::InGUI
			&& playerComponent->preControlState == PlayerControlState::Normal)
		{
			//glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			rigidbodyMotorComponent->movement = glm::vec2(0.0, 0.0);
		}
	}
}