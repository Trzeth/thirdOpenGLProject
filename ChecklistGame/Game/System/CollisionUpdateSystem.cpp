#include "CollisionUpdateSystem.h"

#include "Game/Component/TransformComponent.h"
#include "Game/Component/CollisionComponent.h"

#include "Game/Extra/Util.h"

CollisionUpdateSystem::CollisionUpdateSystem(World& world) :System(world)
{
	require<CollisionComponent>();
	require<TransformComponent>();
}

void CollisionUpdateSystem::updateEntity(float dt, eid_t entity)
{
	CollisionComponent* collisionComponent = world.GetComponent<CollisionComponent>(entity);
	TransformComponent* transformComponent = world.GetComponent<TransformComponent>(entity);

	std::shared_ptr<Transform>& transform = transformComponent->data;
	if (collisionComponent->controlMovement) {
		b2Transform colTransform = collisionComponent->collisionObject->GetTransform();

		glm::vec3 pos = transform->GetWorldPosition();
		glm::quat rot = transform->GetRotation();
		transform->SetPosition(glm::vec3(colTransform.p.x, pos.y, colTransform.p.y));
		transform->SetRotation(glm::angleAxis(colTransform.q.GetAngle(), Transform::UP));
	}
	else
	{
		glm::vec3 pos = transform->GetWorldPosition();
		glm::quat rot = transform->GetWorldRotation();

		collisionComponent->collisionObject->SetTransform(b2Vec2(pos.x, pos.z), glm::eulerAngles(rot).y);
	}
}