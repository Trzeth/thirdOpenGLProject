#include "RigidbodyMotorSystem.h"

#include <glm/gtx/vector_angle.hpp>

#include "Game/Component/CollisionComponent.h"
#include "Game/Component/RigidbodyMotorComponent.h"
#include "Game/Component/TransformComponent.h"

#include "Game/Extra/Util.h"

RigidbodyMotorSystem::RigidbodyMotorSystem(World& world) :System(world)
{
	require<RigidbodyMotorComponent>();
	require<CollisionComponent>();
}

void RigidbodyMotorSystem::updateEntity(float dt, eid_t entity)
{
	RigidbodyMotorComponent* rigidbodyMotorComponent = world.GetComponent<RigidbodyMotorComponent>(entity);
	CollisionComponent* collisionComponent = world.GetComponent<CollisionComponent>(entity);

	if (!rigidbodyMotorComponent->canMove ||
		collisionComponent->collisionObject->GetType() != b2BodyType::b2_dynamicBody)
		return;

	b2Body* body = collisionComponent->collisionObject;

	glm::vec2 movement(0.0f);
	glm::vec2 moveDir = rigidbodyMotorComponent->movement;

	if (glm::length(moveDir) > glm::epsilon<float>()) {
		if (glm::length(moveDir) > 1.0f) {
			moveDir = glm::normalize(moveDir);
		}
		movement = moveDir * rigidbodyMotorComponent->data.moveSpeed;
	}

	glm::vec3 facingVec = rigidbodyMotorComponent->facing * Transform::FORWARD;
	facingVec.y = 0.0f;

	float hFacing = glm::orientedAngle(Transform::FORWARD, glm::normalize(facingVec), Transform::UP);

	b2Vec2 pos = body->GetPosition();
	body->SetTransform(pos, hFacing);

	b2Vec2 velocity = body->GetLinearVelocity();

	if (glm::length(movement) > glm::epsilon<float>()) {
		// Negate Z because forward movement is in the negative direction
		velocity.Set(movement.y, -movement.x);
	}
	else {
		velocity.Set(0.0f, 0.0f);
	}

	if (velocity.Length() > glm::epsilon<float>()) {
		body->SetAwake(true);
	}
	body->SetLinearVelocity(velocity);
}