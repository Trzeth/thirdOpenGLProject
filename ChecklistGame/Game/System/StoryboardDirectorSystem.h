#pragma once
#include <thirdEngine/Framework/System.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Game/Component/StoryboardDirectorComponent.h"

class Renderer;

class StoryboardDirectorSystem :public System
{
public:
	StoryboardDirectorSystem(World& world);

protected:
	void updateEntity(float dt, eid_t entity);

private:
	glm::vec3 interpolate(glm::vec3 a, glm::vec3 b, float lerp);
	glm::quat interpolate(glm::quat a, glm::quat b, float lerp);

	template <class ValType>
	ValType interpolateKeyframes(const std::vector<KeyFrame<ValType>>& keys, float time, unsigned& keyCache);
};
