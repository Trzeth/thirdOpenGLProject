#pragma once
#include <vector>

#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/DefaultComponentConstructor.h>

enum class PlayerState
{
	PlayerState_Normal,
	PlayerState_InStoryboard,
	PlayerState_InGUI
};

enum class PlayerTaskState {
	PlayerTaskState_Finished,
	PlayerTaskState_NotFinished
};

class PlayerComponent :public Component
{
public:
	PlayerComponent() { };

	struct Data
	{
		Data() :camera(World::NullEntity) { }

		eid_t camera;
	};

	Data data;

	PlayerState playerState;
	bool isBusy;

	std::vector<PlayerTaskState> taskState;
};

class PlayerComponentConstructor :public DefaultComponentConstructor<PlayerComponent> {
	using DefaultComponentConstructor<PlayerComponent>::DefaultComponentConstructor;
};
