#pragma once
#include <memory>

#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/ComponentConstructor.h>

#include "ModelViewer/Extra/Transform.h"

struct TransformComponent :public Component
{
	TransformComponent(const Transform& transform) :data(new Transform(transform)) { }
	TransformComponent() :data(new Transform()) { }

	std::shared_ptr<Transform> data;
};

class TransformComponentConstructor :public ComponentConstructor {
public:
	TransformComponentConstructor() :transform() { }
	TransformComponentConstructor(const Transform& transform) : transform(transform) { };

	virtual ComponentConstructorInfo Construct(World& world, eid_t parent, void* userinfo)const;
private:
	Transform transform;
};