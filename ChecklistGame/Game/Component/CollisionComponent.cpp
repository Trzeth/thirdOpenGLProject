#include "CollisionComponent.h"

#include <thirdEngine/Framework/World.h>

#include "Game/Extra/PrefabConstructionInfo.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Extra/Util.h"

ComponentConstructorInfo CollisionComponentConstructor::Construct(World& world, eid_t parent, void* userinfo)
{
	PrefabConstructionInfo* constructionInfo = (PrefabConstructionInfo*)userinfo;

	Transform initialTransform;
	if (constructionInfo != nullptr) {
		initialTransform = constructionInfo->initialTransform;

		if (parent != World::NullEntity) {
			TransformComponent* parentTransformComponent = world.GetComponent<TransformComponent>(parent);
			if (parentTransformComponent != nullptr) {
				initialTransform.SetParent(parentTransformComponent->data);

				if (this->info.controlMovement) {
					printf("WARNING: Collision component controls movement, but is parented to entity with a transform component");
				}
			}
		}
	}

	b2Body* body = this->world.CreateBody(&info.bodyDef);

	glm::vec3 pos = initialTransform.GetPosition();
	glm::vec3 rot = glm::eulerAngles(initialTransform.GetRotation());
	body->SetTransform(b2Vec2(pos.x, pos.z), rot.y);

	for (b2FixtureDef fixture : info.fixtureDefs)
	{
		fixture.filter.categoryBits = (uint16)info.category;
		fixture.filter.maskBits = (uint16)info.mask;
		body->CreateFixture(&fixture);
	}

	CollisionComponent* component = new CollisionComponent();
	component->world = &this->world;
	component->collisionObject = body;
	component->controlMovement = this->info.controlMovement;

	return ComponentConstructorInfo(component, typeid(CollisionComponent).hash_code());
}

void CollisionComponentConstructor::Finish(World& world, eid_t entity)
{
	CollisionComponent* component = world.GetComponent<CollisionComponent>(entity);

	component->collisionObject->GetUserData().pointer = reinterpret_cast<uintptr_t>(new eid_t(entity));

	eid_t id = *reinterpret_cast<eid_t*>(component->collisionObject->GetUserData().pointer);
}