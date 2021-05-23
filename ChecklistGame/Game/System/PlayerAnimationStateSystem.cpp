#include "PlayerAnimationStateSystem.h"

#include <thirdEngine/Renderer/Renderer.h>

#include "Game/Component/PlayerComponent.h"
#include "Game/Component/ModelRenderComponent.h"
#include "Game/Component/TransformComponent.h"

PlayerAnimationStateSystem::PlayerAnimationStateSystem(World& world, Renderer& renderer) :
	System(world), renderer(renderer)
{
	require<ModelRenderComponent>();
	require<PlayerComponent>();
}

void PlayerAnimationStateSystem::updateEntity(float dt, eid_t entity)
{
	ModelRenderComponent* modelRendererComponent = world.GetComponent<ModelRenderComponent>(entity);
	PlayerComponent* playerComponent = world.GetComponent<PlayerComponent>(entity);
	TransformComponent* transformComponent = world.GetComponent<TransformComponent>(entity);

	auto pos = glm::vec3(transformComponent->data->GetWorldPosition());
	std::cout << pos.x << "\t" << pos.y << "\t" << pos.z << std::endl;

	auto& animationTimer = playerComponent->animationTimer;

	// combinedAnim is the default animation name when name define in file is empty

	if (playerComponent->preAnimationState != playerComponent->animationState)
	{
		switch (playerComponent->animationState)
		{
		case PlayerAnimationState::Walk:
			renderer.SetRenderableModelHandle(modelRendererComponent->rendererHandle, playerComponent->data.walk);
			renderer.SetRenderableAnimation(modelRendererComponent->rendererHandle, "combinedAnim");
			break;
		case PlayerAnimationState::WalkWithMower:
			renderer.SetRenderableModelHandle(modelRendererComponent->rendererHandle, playerComponent->data.walk);
			renderer.SetRenderableAnimation(modelRendererComponent->rendererHandle, "combinedAnim");
			break;
		case PlayerAnimationState::PickLetter:
			renderer.SetRenderableModelHandle(modelRendererComponent->rendererHandle, playerComponent->data.pickLetter);
			renderer.SetRenderableAnimation(modelRendererComponent->rendererHandle, "combinedAnim", false);
			break;
		case PlayerAnimationState::PutLetter:
			renderer.SetRenderableModelHandle(modelRendererComponent->rendererHandle, playerComponent->data.putLetter);
			renderer.SetRenderableAnimation(modelRendererComponent->rendererHandle, "combinedAnim", false);
			break;
		case PlayerAnimationState::TurnAround:
			renderer.SetRenderableModelHandle(modelRendererComponent->rendererHandle, playerComponent->data.turnAround);
			renderer.SetRenderableAnimation(modelRendererComponent->rendererHandle, "combinedAnim", false);
			break;
		case PlayerAnimationState::Idle:
		{
			animationTimer = renderer.GetRenderableAnimationTime(modelRendererComponent->rendererHandle);
			if (playerComponent->preAnimationState == PlayerAnimationState::Walk)
			{
				renderer.SetRenderableModelHandle(modelRendererComponent->rendererHandle, playerComponent->data.walk);
				renderer.SetRenderableAnimation(modelRendererComponent->rendererHandle, "combinedAnim", animationTimer, false, false);
			}
			break;
		}
		}
	}

	if (playerComponent->preAnimationState == PlayerAnimationState::Walk
		&& playerComponent->animationState == PlayerAnimationState::Idle)
	{
		if (animationTimer == 0.0f || animationTimer == 0.7f)
		{
			playerComponent->preAnimationState = PlayerAnimationState::Idle;
			renderer.SetRenderableAnimation(modelRendererComponent->rendererHandle, "bindpose");
		}
		else
		{
			float deltaTime = 2 * dt;

			if (animationTimer < 0.7)
			{
				if (animationTimer + deltaTime > 0.7)animationTimer = 0.7;
				else animationTimer += deltaTime;
			}
			else
			{
				if (animationTimer + deltaTime > 1.3)animationTimer = 0.0f;
				else animationTimer += deltaTime;
			}

			renderer.SetRenderableAnimationTime(modelRendererComponent->rendererHandle, animationTimer);
		}
	}
}