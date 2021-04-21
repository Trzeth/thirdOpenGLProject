#pragma once
#include <vector>

#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Renderer/Renderer.h>
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/DefaultComponentConstructor.h>

enum class PlayerControlState
{
	Normal,
	InStoryboard,
	InGUI
};

enum class PlayerTaskState {
	Finished,
	NotFinished
};

enum class PlayerAnimationState {
	Idle,
	Walk,
	WalkWithMower,
	TurnAround,
	PickLetter,
	PutLetter,
	EndPlaceHolder
};

class PlayerComponent :public Component
{
public:
	PlayerComponent()
		:animationState(PlayerAnimationState::EndPlaceHolder),
		preAnimationState(PlayerAnimationState::EndPlaceHolder),
		controlState(PlayerControlState::Normal),
		preControlState(PlayerControlState::Normal),
		animationTimer(0)
	{ };

	struct Data
	{
		Data() :camera(World::NullEntity) { }

		eid_t camera;

		Renderer::ModelHandle walk;
		Renderer::ModelHandle turnAround;
		Renderer::ModelHandle pickLetter;
		Renderer::ModelHandle putLetter;
	};

	Data data;

	void SetAnimationState(PlayerAnimationState newState)
	{
		preAnimationState = animationState;
		animationState = newState;
	}

	void SetControlState(PlayerControlState newState)
	{
		preControlState = controlState;
		controlState = newState;
	}

private:
	friend class PlayerAnimationStateSystem;
	friend class PlayerControlStateSystem;
	friend class PlayerInputSystem;

	PlayerAnimationState animationState;
	PlayerAnimationState preAnimationState;

	PlayerControlState controlState;
	PlayerControlState preControlState;

	float animationTimer;

	bool isBusy;
	std::vector<PlayerTaskState> taskState;
};

class PlayerComponentConstructor :public DefaultComponentConstructor<PlayerComponent> {
	using DefaultComponentConstructor<PlayerComponent>::DefaultComponentConstructor;
};
