#pragma once
#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Storyboard.h"

class StoryboardLoader
{
public:
	std::vector<Storyboard> LoadFromFile(const std::string& filepath);

private:
	template<typename T>
	std::vector<T> parseJsonArray(const rapidjson::GenericArray<true, rapidjson::Value>& arr);
	template<>
	std::vector<KeyFrame<glm::vec3>> parseJsonArray(const rapidjson::GenericArray<true, rapidjson::Value>& arr);
	template<>
	std::vector<KeyFrame<glm::quat>> parseJsonArray(const rapidjson::GenericArray<true, rapidjson::Value>& arr);
};
