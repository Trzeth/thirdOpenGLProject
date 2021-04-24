#pragma once
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/DefaultComponentConstructor.h>

class InteractComponent :public Component
{
public:
	InteractComponent() { }

	struct Data
	{
		Data() :eventHashCode() { }
		Data(size_t hashCode) :eventHashCode(hashCode) { }

		size_t eventHashCode;
	};

	Data data;
};

class InteractComponentConstructor : public DefaultComponentConstructor<InteractComponent> {
	using DefaultComponentConstructor<InteractComponent>::DefaultComponentConstructor;
};