#pragma once
#include <unordered_map>

#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/DefaultComponentConstructor.h>

#include "Game/Storyboard/Storyboard.h"

enum class StoryboardState
{
	//Set to begin
	BeginFlag,
	Running,
	End
};

enum class StoryboardLoopType {
	Disable,
	Forever
};

/*! Data specific to a channel for a specific model. */
struct AnimatedElementContext
{
	/*! The last position key we used. */
	unsigned positionKey;

	/*! The last rotation key we used. */
	unsigned rotationKey;

	/*! The last scale key we used. */
	unsigned scaleKey;
};

class StoryboardDirectorComponent :public Component
{
public:
	StoryboardDirectorComponent() { };

	struct Data {
		Data() :storyboardState(StoryboardState::End), loopType(StoryboardLoopType::Disable), currentStoryboard(), nameIdMap() { };

		StoryboardState storyboardState;
		StoryboardLoopType loopType;
		Storyboard currentStoryboard;

		//AnimatedElement Name => Entity ID
		std::unordered_map<std::string, eid_t> nameIdMap;
	};

	Data data;

	float currentTime;

	//Reset When System read BeginFlag
	std::unordered_map<eid_t, AnimatedElementContext> animatedElementContexts;
};

class StoryboardDirectorComponentConstructor :public DefaultComponentConstructor<StoryboardDirectorComponent> {
	using DefaultComponentConstructor<StoryboardDirectorComponent>::DefaultComponentConstructor;
};
