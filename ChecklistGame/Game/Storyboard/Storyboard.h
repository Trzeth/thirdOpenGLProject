#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

template<typename T>
struct KeyFrame {
	float time;
	T value;
	KeyFrame(T val, float t) :time(t), value(val) { }
	KeyFrame() :time(), value() { }
};

struct Storyboard
{
	struct AnimatedElement {
		std::vector<KeyFrame<glm::vec3>> PositionChannel;
		std::vector<KeyFrame<glm::quat>> RotationChannel;
		std::vector<KeyFrame<glm::vec3>> ScaleChannel;
	};

	//Un-used
	float StartTime;

	float EndTime;
	std::unordered_map<std::string, AnimatedElement> AnimatedElementList;
	std::string Name;
};
