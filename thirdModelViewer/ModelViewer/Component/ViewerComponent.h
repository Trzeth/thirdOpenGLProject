#pragma once
#include <vector>

#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/DefaultComponentConstructor.h>

enum class ViewerState
{
	FPSCamera,
	FixedCamera
};

class ViewerComponent :public Component
{
public:
	ViewerComponent() {  };

	struct Data
	{
		Data() :FPSCamera(World::NullEntity), FixedCamera(World::NullEntity) {};
		eid_t FPSCamera;
		eid_t FixedCamera;
	};

	Data data;

	ViewerState viewerState;
};

class ViewerComponentConstructor :public DefaultComponentConstructor<ViewerComponent> {
	using DefaultComponentConstructor<ViewerComponent>::DefaultComponentConstructor;
};
