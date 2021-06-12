#include "CharacterShowcaseSystem.h"

#include <thirdEngine/Input/Input.h>

#include "Game/Component/CharacterShowcaseComponent.h"
#include "Game/Component/TransformComponent.h"

CharacterShowcaseSystem::CharacterShowcaseSystem(World& world, Input& input, Renderer& renderer)
	:System(world), input(input), renderer(renderer)
{
	require<CharacterShowcaseComponent>();
	require<TransformComponent>();
}

void CharacterShowcaseSystem::updateEntity(float dt, eid_t entity)
{
	TransformComponent* playerTransformComponent = world.GetComponent<TransformComponent>(entity);

	bool press = input.GetButtonDown("LeftMouse", Device_Kbm);
	float lookHorizontal = input.GetAxis("LookHorizontal", Device_Kbm);
	float lookVertical = input.GetAxis("LookVertical", Device_Kbm);

	horizontalRad -= lookHorizontal * dt;

	playerTransformComponent->data->SetRotation(glm::angleAxis(glm::pi<float>() / 2 - horizontalRad, Transform::UP));
}