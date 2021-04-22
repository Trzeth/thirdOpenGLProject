#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/DefaultComponentConstructor.h>

class RigidbodyMotorComponent :public Component
{
public:
	RigidbodyMotorComponent() :canMove(true), facing(), movement(), numContacts(0) { }

	struct Data {
		Data(float moveSpeed = 4.5f) :moveSpeed(moveSpeed) { }

		float moveSpeed;
	};

	Data data;

	glm::quat facing;
	glm::vec2 movement;

	bool canMove;

	unsigned int numContacts;
};

class RigidbodyMotorComponentConstructor :public DefaultComponentConstructor<RigidbodyMotorComponent> {
public:
	using DefaultComponentConstructor<RigidbodyMotorComponent>::DefaultComponentConstructor;
};
