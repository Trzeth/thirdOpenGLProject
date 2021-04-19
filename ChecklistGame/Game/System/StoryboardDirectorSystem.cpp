#include "StoryboardDirectorSystem.h"

#include <thirdEngine/Renderer/Renderer.h>

#include "Game/Component/TransformComponent.h"

StoryboardDirectorSystem::StoryboardDirectorSystem(World& world) :System(world)
{
	require<StoryboardDirectorComponent>();
}

glm::vec3 StoryboardDirectorSystem::interpolate(glm::vec3 a, glm::vec3 b, float lerp)
{
	return glm::mix(a, b, lerp);
}

glm::quat StoryboardDirectorSystem::interpolate(glm::quat a, glm::quat b, float lerp)
{
	return glm::slerp(a, b, lerp);
}

template <class ValType>
ValType StoryboardDirectorSystem::interpolateKeyframes(const std::vector<KeyFrame<ValType>>& keys, float time, unsigned& keyCache)
{
	unsigned keysSize = keys.size();
	unsigned keyIndex = keysSize - 1;

	if (time <= keys[0].time) {
		keyIndex = 0;
	}
	else if (time >= keys[keysSize - 1].time) {
		keyIndex = keysSize - 1;
	}
	else {
		for (unsigned int i = keyCache; i < keyCache + keysSize - 1; i++) {
			const KeyFrame<ValType>& ka = keys[i % keysSize];
			const KeyFrame<ValType>& kb = (i + 1 != keysSize ? keys[(i + 1) % keysSize] : ka);
			if (ka.time <= time && time <= kb.time) {
				keyIndex = i % keysSize;
				break;
			}
		}
	}

	keyCache = keyIndex;
	float t1 = keys[keyIndex].time;
	float t2 = (keyIndex + 1 < keysSize ? keys[keyIndex + 1].time : t1);
	const ValType& p1 = keys[keyIndex].value;
	const ValType& p2 = (keyIndex + 1 < keysSize ? keys[keyIndex + 1].value : p1);

	float lerp = (time - t1) / (t2 - t1);
	if (t2 == t1) {
		lerp = 1.0f;
	}

	return interpolate(p1, p2, lerp);
}

void StoryboardDirectorSystem::updateEntity(float dt, eid_t entity)
{
	StoryboardDirectorComponent* storyboardDir = world.GetComponent<StoryboardDirectorComponent>(entity);

	const Storyboard& sb = storyboardDir->data.currentStoryboard;
	float& currentTime = storyboardDir->currentTime;

	if (storyboardDir->data.storyboardState == StoryboardState::End)
		return;

	if (storyboardDir->data.storyboardState == StoryboardState::BeginFlag) {
		//Init component val
		storyboardDir->animatedElementContexts.clear();

		for (const auto& keypair : storyboardDir->data.nameIdMap) {
			auto it = sb.AnimatedElementList.find(keypair.first);
			if (it != sb.AnimatedElementList.end()) {
				storyboardDir->animatedElementContexts.emplace(keypair.second, AnimatedElementContext());
			}
		}

		storyboardDir->data.storyboardState = StoryboardState::Running;
	}

	float duration = sb.EndTime - sb.StartTime;
	if (storyboardDir->currentTime >= duration) {
		if (storyboardDir->data.loopType == StoryboardLoopType::Disable) {}
		if (storyboardDir->data.loopType == StoryboardLoopType::Forever) {
			storyboardDir->currentTime -= duration;
		}
	}

	for (const auto& keypair : storyboardDir->data.nameIdMap) {
		auto it = sb.AnimatedElementList.find(keypair.first);
		if (it != sb.AnimatedElementList.end()) {
			TransformComponent* transformComponet = world.GetComponent<TransformComponent>(keypair.second);
			const auto& animatedEle = it->second;
			if (animatedEle.PositionChannel.size() > 0)
			{
				transformComponet->data->SetPosition(
					interpolateKeyframes<glm::vec3>(
						animatedEle.PositionChannel,
						currentTime,
						storyboardDir->animatedElementContexts[keypair.second].positionKey));
			}
			if (animatedEle.RotationChannel.size() > 0)
			{
				transformComponet->data->SetRotation(
					interpolateKeyframes<glm::quat>(
						animatedEle.RotationChannel,
						currentTime,
						storyboardDir->animatedElementContexts[keypair.second].scaleKey));
			}
			if (animatedEle.ScaleChannel.size() > 0)
			{
				transformComponet->data->SetScale(
					interpolateKeyframes<glm::vec3>(
						animatedEle.ScaleChannel,
						currentTime,
						storyboardDir->animatedElementContexts[keypair.second].rotationKey));
			}
		}
	}

	storyboardDir->currentTime += dt;
}