#pragma
#include <box2d/box2d.h>
#include <memory>
#include <vector>

#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/ComponentConstructor.h>

enum class CollisionCategory {
	Default = 1,
	Enemy = 1 << 1,
	Player = 1 << 2,
	Wall = 1 << 3,
	Hurtbox = 1 << 4,
	All = Default | Enemy | Player | Wall | Hurtbox
};

struct CollisionConstructorInfo {
	CollisionConstructorInfo(const b2BodyDef& bodyDef, const std::vector<b2FixtureDef>& fixtureDefs, CollisionCategory category, CollisionCategory mask, bool controlMovement)
		: bodyDef(bodyDef), fixtureDefs(fixtureDefs), collisionFlags(0), category(category), mask(mask), controlMovement(controlMovement) {	}
	CollisionConstructorInfo(const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDefs)
		: CollisionConstructorInfo(bodyDef, std::vector<b2FixtureDef>{fixtureDefs}, CollisionCategory::Default, CollisionCategory::All, true) { }
	CollisionConstructorInfo(const b2BodyDef& bodyDef, const std::vector<b2FixtureDef>& fixtureDefs)
		: CollisionConstructorInfo(bodyDef, fixtureDefs, CollisionCategory::Default, CollisionCategory::All, true) { }

	b2BodyDef bodyDef;
	std::vector<b2FixtureDef> fixtureDefs;

	int collisionFlags;
	CollisionCategory category;
	CollisionCategory mask;

	bool controlMovement;
};

struct CollisionComponent :public Component
{
	CollisionComponent() : world(nullptr), collisionObject(nullptr), controlMovement(true) { }
	~CollisionComponent()
	{
		if (world != nullptr) {
			world->DestroyBody(this->collisionObject);
		}
	}

	b2World* world;
	b2Body* collisionObject;
	bool controlMovement;
};

class CollisionComponentConstructor :public ComponentConstructor {
public:
	CollisionComponentConstructor(b2World& world, const CollisionConstructorInfo& info)
		: world(world), info(info) { }

	virtual ComponentConstructorInfo Construct(World& world, eid_t parent, void* userinfo);

	virtual void Finish(World& world, eid_t entity);
private:
	b2World& world;
	CollisionConstructorInfo info;
};
