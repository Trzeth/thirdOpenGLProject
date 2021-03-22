#include "Input.h"

Input::Input() :kbmDevice(), kbmMapping()
{ }

void Input::Initialize()
{ }

void Input::Update()
{
	kbmDevice.Update();
}

void Input::HandleKeyCallBack(int key, int scancode, int action, int mods)
{
	kbmDevice.HandleKeyCallBack(key, scancode, action, mods);
}

void Input::HandleMouseButtonCallBack(int button, int action, int mods)
{
	kbmDevice.HandleMouseButtonCallBack(button, action, mods);
}

void Input::HandleScrollCallback(double xoffset, double yoffset)
{
	kbmDevice.HandleScrollCallback(xoffset, yoffset);
}

void Input::HandleMouseCallback(double xpos, double ypos)
{
	kbmDevice.HandleMouseCallback(xpos, ypos);
}

float Input::GetAxis(const std::string& axis, Device device)
{
	return simulateAxis(axis, device);
}

bool Input::GetButtonDown(const std::string& button, Device device)
{
	AxisProps props = getAxisProperties(button, device);
	float currentValue = simulateAxis(button, device, false);
	float previousValue = simulateAxis(button, device, true);
	return currentValue > props.pressedThreshold&& previousValue < props.pressedThreshold;
}

bool Input::GetButtonUp(const std::string& button, Device device)
{
	AxisProps props = getAxisProperties(button, device);
	float currentValue = simulateAxis(button, device, false);
	float previousValue = simulateAxis(button, device, true);
	return currentValue < props.pressedThreshold && previousValue > props.pressedThreshold;
}

bool Input::GetButton(const std::string& button, Device device)
{
	AxisProps props = getAxisProperties(button, device);
	float currentValue = simulateAxis(button, device, false);
	return currentValue > props.pressedThreshold;
}

void Input::SetDefaultMapping(const std::string& action, KbmAxis positive, KbmAxis negative, AxisProps props)
{
	kbmMapping.SetMapping(action, InputMapper<KbmAxis>::Axis(positive, negative, props), true);
}

void Input::SetMapping(const std::string& action, KbmAxis positive, KbmAxis negative, AxisProps props)
{
	kbmMapping.SetMapping(action, InputMapper<KbmAxis>::Axis(positive, negative, props), false);
}

AxisProps Input::getAxisProperties(const std::string& action, Device device)
{
	if (device == Device_Kbm) {
		return kbmMapping.GetMappingFor(action).props;
	}
	throw "Invalid Device";

	return AxisProps();
}

float Input::simulateAxis(const std::string& action, Device device, bool previous)
{
	float rawAxisValue;
	AxisProps axisProps;
	if (device == Device_Kbm) {
		InputMapper<KbmAxis>::Axis axis = kbmMapping.GetMappingFor(action);
		rawAxisValue = kbmDevice.GetAxis(axis.positive, previous) - kbmDevice.GetAxis(axis.negative, previous);
		axisProps = axis.props;
	}

	if (std::abs(rawAxisValue) < axisProps.deadZone)
		return 0.0f;

	return axisProps.sensitivity * rawAxisValue;
}