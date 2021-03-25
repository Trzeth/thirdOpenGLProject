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

class ObjectViewerComponent :public Component
{
public:
	ObjectViewerComponent() :viewerState(ViewerState::FPSCamera) {  };

	struct Data
	{
		Data() :FPSCamera(World::NullEntity), FixedCamera(World::NullEntity) {};
		eid_t FPSCamera;
		eid_t FixedCamera;
	};

	Data data;

	ViewerState viewerState;
};

class ObjectViewerComponentConstructor :public DefaultComponentConstructor<ObjectViewerComponent> {
	using DefaultComponentConstructor<ObjectViewerComponent>::DefaultComponentConstructor;
};
