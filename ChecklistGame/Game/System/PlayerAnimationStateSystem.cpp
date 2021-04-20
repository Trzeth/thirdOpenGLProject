#include "PlayerAnimationStateSystem.h"

#include <thirdEngine/Renderer/Renderer.h>

#include "Game/Component/PlayerComponent.h"
#include "Game/Component/ModelRenderComponent.h"

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
			if (playerComponent->preAnimationState == PlayerAnimationState::Walk) {
				renderer.SetRenderableAnimation(modelRendererComponent->rendererHandle, "combinedAnim");
			}
			else
			{
				renderer.SetRenderableAnimation(modelRendererComponent->rendererHandle, "bindpose");
			}
			break;
		}
		}
	}
}