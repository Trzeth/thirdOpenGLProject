#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>

#include "KbmDevice.h"

enum Device {
	Device_Kbm
};

struct AxisProps
{
	AxisProps(float sensitivity, float deadZone, float pressedThreshold)
		:sensitivity(sensitivity), deadZone(deadZone), pressedThreshold(pressedThreshold) { }
	AxisProps() :sensitivity(1.0f), deadZone(0.2f), pressedThreshold(0.4f) { };

	float sensitivity;
	float deadZone;
	float pressedThreshold;
};

template<typename T>
class InputMapper {
public:
	struct Axis
	{
		Axis() :positive((T)0), negative((T)0) { };
		Axis(T positive, T negative, AxisProps props)
			:positive(positive), negative(negative), props(props) { }

		T positive;
		T negative;
		AxisProps props;
	};

	Axis GetMappingFor(const std::string& action)const;
	void SetMapping(const std::string& action, Axis axis, bool isDefault = false);
	void SetAxisProps(const std::string& action, AxisProps props, bool isDefault = false);
private:
	std::unordered_map<std::string, Axis> defaultInputMap;
	std::unordered_map<std::string, Axis> inputMap;
};

class Input
{
public:
	Input();
	void Initialize();
	void Update();

	void HandleKeyCallBack(int key, int scancode, int action, int mods);
	void HandleMouseButtonCallBack(int button, int action, int mods);
	void HandleScrollCallback(double xoffset, double yoffset);
	void HandleMouseCallback(double xpos, double ypos);

	float GetAxis(const std::string& axis, Device device);
	bool GetButtonDown(const std::string& button, Device device);
	bool GetButtonUp(const std::string& button, Device device);
	bool GetButton(const std::string& button, Device device);

	void SetDefaultMapping(const std::string& action, KbmAxis positive, KbmAxis negative, AxisProps props = AxisProps());
	void SetMapping(const std::string& action, KbmAxis positive, KbmAxis negative, AxisProps props = AxisProps());

private:
	KbmDevice kbmDevice;
	InputMapper<KbmAxis> kbmMapping;

	AxisProps getAxisProperties(const std::string& action, Device device);
	float simulateAxis(const std::string& action, Device device, bool previous = false);
};

template<typename T>
typename InputMapper<T>::Axis InputMapper<T>::GetMappingFor(const std::string& action) const
{
	auto iter = inputMap.find(action);
	if (iter != inputMap.end()) {
		return iter->second;
	}

	auto defaultIter = defaultInputMap.find(action);
	if (defaultIter != defaultInputMap.end()) {
		return defaultIter->second;
	}

	throw "Non-exist Action";
}

template<typename T>
void InputMapper<T>::SetMapping(const std::string& action, Axis axis, bool isDefault)
{
	if (isDefault) {
		defaultInputMap[action] = axis;
	}
	else
	{
		inputMap[action] = axis;
	}
}

template<typename T>
void InputMapper<T>::SetAxisProps(const std::string& action, AxisProps props, bool isDefault)
{
	if (isDefault) {
		auto iter = defaultInputMap.find(action);
		if (iter == defaultInputMap.end()) {
			return;
		}
		iter->second.props = props;
	}
	else
	{
		auto iter = inputMap.find(action);
		if (iter == inputMap.end()) {
			return;
		}
		iter->second.props = props;
	}
}
