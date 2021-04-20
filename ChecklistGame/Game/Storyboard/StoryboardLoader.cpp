#include "StoryboardLoader.h"

#include <string>
#include <sstream>
#include <thirdEngine/Renderer/Shader.h>

using namespace std;
using namespace rapidjson;

std::vector<Storyboard> StoryboardLoader::LoadFromFile(const std::string& filepath)
{
	string sbCode;
	ifstream fs;
	fs.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		fs.open(filepath);
		stringstream ss;
		ss << fs.rdbuf();
		fs.close();
		sbCode = ss.str();
	}
	catch (ifstream::failure e)
	{
		printf("ERROR::STORYBOARD::FILE_NOT_SUCCESFULLY_READ\n");
	}

	Document sbDocument;

	if (sbDocument.Parse(sbCode.c_str()).HasParseError()) {
		printf("ERROR::STORYBOARD::FILE_NOT_SUCCESFULLY_PARSE\n");
	}

	//Must be Storyboard array
	assert(sbDocument.IsArray());

	std::vector<Storyboard> sbs;
	for (Value::ConstValueIterator it = sbDocument.Begin(); it != sbDocument.End(); it++) {
		Storyboard sb;
		auto obj = it->GetObject();
		sb.Name = obj["Name"].GetString();
		sb.StartTime = obj["StartTime"].GetFloat();

		for (auto& v : obj["AnimatedElements"].GetArray()) {
			auto obj = v.GetObject();
			string name = v["Name"].GetString();
			Storyboard::AnimatedElement animatedEle;
			if (obj.HasMember("PositionChannel"))animatedEle.PositionChannel = parseJsonArray<KeyFrame<glm::vec3>>(obj["PositionChannel"].GetArray());
			if (obj.HasMember("RotationChannel"))animatedEle.RotationChannel = parseJsonArray<KeyFrame<glm::quat>>(obj["RotationChannel"].GetArray());
			if (obj.HasMember("ScaleChannel"))animatedEle.ScaleChannel = parseJsonArray<KeyFrame<glm::vec3>>(obj["ScaleChannel"].GetArray());
			if (obj.HasMember("PlayerAnimationStateChannel")) animatedEle.PlayerAnimationStateChannel = parseJsonArray<KeyFrame<PlayerAnimationState>>(obj["PlayerAnimationStateChannel"].GetArray());

			sb.AnimatedElementList.emplace(name, animatedEle);
		}

		float max = sb.StartTime;
		for (auto& v : sb.AnimatedElementList) {
			if (v.second.PositionChannel.size() > 0)max = std::max(v.second.PositionChannel.rbegin()->time, max);
			if (v.second.RotationChannel.size() > 0)max = std::max(v.second.RotationChannel.rbegin()->time, max);
			if (v.second.ScaleChannel.size() > 0)max = std::max(v.second.ScaleChannel.rbegin()->time, max);
			if (v.second.PlayerAnimationStateChannel.size() > 0)max = std::max(v.second.PlayerAnimationStateChannel.rbegin()->time, max);
		}

		sb.EndTime = max;

		sbs.push_back(sb);
	}

	return sbs;
}

template<>
inline std::vector<KeyFrame<glm::vec3>> StoryboardLoader::parseJsonArray(const GenericArray<true, Value>& arr)
{
	std::vector<KeyFrame<glm::vec3>> v;
	for (auto& obj : arr) {
		auto val = obj["Value"].GetArray();
		assert(val.Size() == 3);
		v.push_back(KeyFrame<glm::vec3>(glm::vec3(val[0].GetFloat(), val[1].GetFloat(), val[2].GetFloat()), obj["Time"].GetFloat()));
	}

	return v;
}

template<>
inline std::vector<KeyFrame<glm::quat>> StoryboardLoader::parseJsonArray(const GenericArray<true, Value>& arr)
{
	std::vector<KeyFrame<glm::quat>> v;
	for (auto& obj : arr) {
		auto val = obj["Value"].GetArray();

		assert(val.Size() == 3 || val.Size() == 4);
		if (val.Size() == 3)
		{
			//Yaw Pitch Roll
			glm::vec3 UP(0, 1, 0), RIGHT(1, 0, 0), FRONT(0, 0, -1);
			glm::quat q = glm::angleAxis(glm::radians(val[0].GetFloat()), UP) * glm::angleAxis(glm::radians(val[1].GetFloat()), RIGHT) * glm::angleAxis(glm::radians(val[2].GetFloat()), FRONT);
			v.push_back(KeyFrame<glm::quat>(q, obj["Time"].GetFloat()));
		}
		else
		{
			// quat
			v.push_back(KeyFrame<glm::quat>(glm::quat(val[0].GetFloat(), val[1].GetFloat(), val[2].GetFloat(), val[3].GetFloat()), obj["Time"].GetFloat()));
		}
	}

	return v;
}

template<>
inline std::vector<KeyFrame<PlayerAnimationState>> StoryboardLoader::parseJsonArray(const GenericArray<true, Value>& arr)
{
	std::vector<KeyFrame<PlayerAnimationState>> v;
	for (auto& obj : arr) {
		assert(obj["Value"].IsString());

		auto val = string(obj["Value"].GetString());
		PlayerAnimationState state = PlayerAnimationState::EndPlaceHolder;

		if (val == "Walk") {
			state = PlayerAnimationState::Walk;
		}
		else if (val == "PickLetter") {
			state = PlayerAnimationState::PickLetter;
		}
		else if (val == "PutLetter") {
			state = PlayerAnimationState::PutLetter;
		}
		else if (val == "TurnAround") {
			state = PlayerAnimationState::TurnAround;
		}
		else if (val == "WalkWithMower") {
			state = PlayerAnimationState::WalkWithMower;
		}
		else if (val == "Idle") {
			state = PlayerAnimationState::Idle;
		}
		else if (val == "EndPlaceHolder") {
			state = PlayerAnimationState::EndPlaceHolder;
		}

		v.push_back(KeyFrame<PlayerAnimationState>(state, obj["Time"].GetFloat()));
	}

	return v;
}