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

	unsigned playerAnimationStateKey;
};

class StoryboardDirectorComponent :public Component
{
public:
	struct Data {
		Data() :storyboardState(StoryboardState::End), loopType(StoryboardLoopType::Disable), currentStoryboard(), nameIdMap() { };
		Data(Storyboard sb, std::unordered_map<std::string, eid_t> nameIdMap, StoryboardLoopType loopType = StoryboardLoopType::Disable)
			:currentStoryboard(sb), nameIdMap(nameIdMap), loopType(loopType)
		{
			storyboardState = StoryboardState::BeginFlag;
		}

		StoryboardState storyboardState;
		StoryboardLoopType loopType;
		Storyboard currentStoryboard;

		//AnimatedElement Name => Entity ID
		std::unordered_map<std::string, eid_t> nameIdMap;
	};

	StoryboardDirectorComponent() :data(), currentTime(0), animatedElementContexts(), endEventHashCode(0)
	{ }

	void BeginStoryboard(Data d, float startTime = 0)
	{
		data = d;
		currentTime = startTime;
		animatedElementContexts.clear();
	};

	/*!
	 * @brief BeginStoryboard With end event
	 * @tparam T
	 * @param d
	 * @param startTime
	*/
	template<class T>
	void BeginStoryboard(Data d, float startTime = 0)
	{
		endEventHashCode = typeid(T).hash_code();
		BeginStoryboard(d, startTime);
	}

private:
	friend class StoryboardDirectorSystem;
	friend class DefaultComponentConstructor<StoryboardDirectorComponent>;
	Data data;

	size_t endEventHashCode;
	float currentTime;

	//Reset When System read BeginFlag
	std::unordered_map<eid_t, AnimatedElementContext> animatedElementContexts;
};

class StoryboardDirectorComponentConstructor :public DefaultComponentConstructor<StoryboardDirectorComponent> {
	using DefaultComponentConstructor<StoryboardDirectorComponent>::DefaultComponentConstructor;
};
