#include <GLFW/glfw3.h>

#include "KbmDevice.h"

#define ACTIONSTATUS (action == GLFW_PRESS ? 1.0f : 0.0f)

KbmDevice::KbmDevice()
{ }

void KbmDevice::Update()
{
	for (auto iter = axisDataMap.begin(); iter != axisDataMap.end(); ++iter) {
		if (iter->first >= KbmAxis_MouseXPos && iter->first <= KbmAxis_MouseScrollYNeg) {
			AxisData& data = iter->second;
			data.previousValue = data.currentValue;
			data.currentValue = data.pendingValue;
			data.pendingValue = 0.0f;
		}
	}
}

void KbmDevice::HandleKeyCallBack(int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT)return;

	KbmAxis axis = axisFromGLFW(key);

	if (axis != KbmAxis_None) {
		axisDataMap[axis].previousValue = axisDataMap[axis].currentValue;
		axisDataMap[axis].currentValue = ACTIONSTATUS;
	}

	//不使用参数mods进行判定
	axis = modAxisFromGLFW(key);
	if (axis != KbmAxis_None) {
		axisDataMap[axis].previousValue = axisDataMap[axis].currentValue;
		axisDataMap[axis].currentValue = ACTIONSTATUS;
	}
}

void KbmDevice::HandleMouseButtonCallBack(int button, int action, int mods)
{
	if (action == GLFW_REPEAT)return;

	KbmAxis axis = mouseAxisFromGLFW(button);
	if (axis != KbmAxis_None) {
		axisDataMap[axis].previousValue = axisDataMap[axis].currentValue;
		axisDataMap[axis].currentValue = ACTIONSTATUS;
	}
}

void KbmDevice::HandleScrollCallback(double xoffset, double yoffset)
{
	axisDataMap[KbmAxis_MouseScrollXPos].pendingValue += (xoffset > 0.0f ? xoffset : 0.0f);
	axisDataMap[KbmAxis_MouseScrollXNeg].pendingValue -= (xoffset < 0.0f ? xoffset : 0.0f);
	axisDataMap[KbmAxis_MouseScrollYPos].pendingValue += (yoffset > 0.0f ? yoffset : 0.0f);
	axisDataMap[KbmAxis_MouseScrollYNeg].pendingValue -= (yoffset < 0.0f ? yoffset : 0.0f);
}

void KbmDevice::HandleMouseCallback(double xpos, double ypos)
{
	// Invert the Y axis because down is positive, and we want up to be positive
	axisDataMap[KbmAxis_MouseXPos].pendingValue += (xpos > 0.0f ? xpos : 0.0f);
	axisDataMap[KbmAxis_MouseXNeg].pendingValue -= (xpos < 0.0f ? xpos : 0.0f);
	axisDataMap[KbmAxis_MouseYPos].pendingValue -= (ypos < 0.0f ? ypos : 0.0f);
	axisDataMap[KbmAxis_MouseYNeg].pendingValue += (ypos > 0.0f ? ypos : 0.0f);
}

float KbmDevice::GetAxis(KbmAxis axis, bool previous)
{
	auto iter = axisDataMap.find(axis);
	if (iter == axisDataMap.end()) {
		return 0.0f;
	}

	if (previous) {
		return iter->second.previousValue;
	}

	return iter->second.currentValue;
}

KbmAxis KbmDevice::axisFromGLFW(int keycode)
{
	KbmAxis axis;
	switch (keycode)
	{
	case GLFW_KEY_UNKNOWN:axis = KbmAxis_None; break;
	case GLFW_KEY_SPACE:axis = KbmAxis_Space; break;
	case GLFW_KEY_APOSTROPHE:axis = KbmAxis_Quote; break;
	case GLFW_KEY_COMMA:axis = KbmAxis_Comma; break;
	case GLFW_KEY_MINUS:axis = KbmAxis_Minus; break;
	case GLFW_KEY_PERIOD:axis = KbmAxis_Period; break;
	case GLFW_KEY_SLASH:axis = KbmAxis_Slash; break;
	case GLFW_KEY_0:axis = KbmAxis_Key0; break;
	case GLFW_KEY_1:axis = KbmAxis_Key1; break;
	case GLFW_KEY_2:axis = KbmAxis_Key2; break;
	case GLFW_KEY_3:axis = KbmAxis_Key3; break;
	case GLFW_KEY_4:axis = KbmAxis_Key4; break;
	case GLFW_KEY_5:axis = KbmAxis_Key5; break;
	case GLFW_KEY_6:axis = KbmAxis_Key6; break;
	case GLFW_KEY_7:axis = KbmAxis_Key7; break;
	case GLFW_KEY_8:axis = KbmAxis_Key8; break;
	case GLFW_KEY_9:axis = KbmAxis_Key9; break;
	case GLFW_KEY_SEMICOLON:axis = KbmAxis_Semicolon; break;
	case GLFW_KEY_EQUAL:axis = KbmAxis_Equals; break;
	case GLFW_KEY_A:axis = KbmAxis_A; break;
	case GLFW_KEY_B:axis = KbmAxis_B; break;
	case GLFW_KEY_C:axis = KbmAxis_C; break;
	case GLFW_KEY_D:axis = KbmAxis_D; break;
	case GLFW_KEY_E:axis = KbmAxis_E; break;
	case GLFW_KEY_F:axis = KbmAxis_F; break;
	case GLFW_KEY_G:axis = KbmAxis_G; break;
	case GLFW_KEY_H:axis = KbmAxis_H; break;
	case GLFW_KEY_I:axis = KbmAxis_I; break;
	case GLFW_KEY_J:axis = KbmAxis_J; break;
	case GLFW_KEY_K:axis = KbmAxis_K; break;
	case GLFW_KEY_L:axis = KbmAxis_L; break;
	case GLFW_KEY_M:axis = KbmAxis_M; break;
	case GLFW_KEY_N:axis = KbmAxis_N; break;
	case GLFW_KEY_O:axis = KbmAxis_O; break;
	case GLFW_KEY_P:axis = KbmAxis_P; break;
	case GLFW_KEY_Q:axis = KbmAxis_Q; break;
	case GLFW_KEY_R:axis = KbmAxis_R; break;
	case GLFW_KEY_S:axis = KbmAxis_S; break;
	case GLFW_KEY_T:axis = KbmAxis_T; break;
	case GLFW_KEY_U:axis = KbmAxis_U; break;
	case GLFW_KEY_V:axis = KbmAxis_V; break;
	case GLFW_KEY_W:axis = KbmAxis_W; break;
	case GLFW_KEY_X:axis = KbmAxis_X; break;
	case GLFW_KEY_Y:axis = KbmAxis_Y; break;
	case GLFW_KEY_Z:axis = KbmAxis_Z; break;
	case GLFW_KEY_LEFT_BRACKET:axis = KbmAxis_LeftBracket; break;
	case GLFW_KEY_BACKSLASH:axis = KbmAxis_Backslash; break;
	case GLFW_KEY_RIGHT_BRACKET:axis = KbmAxis_RightBracket; break;
		//case GLFW_KEY_GRAVE_ACCENT:axis = KbmAxis_Comma; break;
		//case GLFW_KEY_WORLD_1:axis = KbmAxis_Comma; break;
		//case GLFW_KEY_WORLD_2:axis = KbmAxis_Comma; break;
	case GLFW_KEY_ESCAPE:axis = KbmAxis_Escape; break;
	case GLFW_KEY_ENTER:axis = KbmAxis_Return; break;
	case GLFW_KEY_TAB:axis = KbmAxis_Tab; break;
	case GLFW_KEY_BACKSPACE:axis = KbmAxis_Backspace; break;
	case GLFW_KEY_INSERT:axis = KbmAxis_Insert; break;
	case GLFW_KEY_DELETE:axis = KbmAxis_Delete; break;
	case GLFW_KEY_RIGHT:axis = KbmAxis_RightArrow; break;
	case GLFW_KEY_LEFT:axis = KbmAxis_LeftArrow; break;
	case GLFW_KEY_DOWN:axis = KbmAxis_DownArrow; break;
	case GLFW_KEY_UP:axis = KbmAxis_UpArrow; break;
	case GLFW_KEY_PAGE_UP:axis = KbmAxis_PageUp; break;
	case GLFW_KEY_PAGE_DOWN:axis = KbmAxis_PageDown; break;
	case GLFW_KEY_HOME:axis = KbmAxis_Tab; break;
	case GLFW_KEY_END:axis = KbmAxis_End; break;
		//case GLFW_KEY_CAPS_LOCK:axis = KbmAxis_Tab; break;
		//case GLFW_KEY_SCROLL_LOCK:axis = KbmAxis_Tab; break;
	case GLFW_KEY_NUM_LOCK:axis = KbmAxis_Numlock; break;
		//case GLFW_KEY_PRINT_SCREEN:axis = KbmAxis_Tab; break;
		//case GLFW_KEY_PAUSE:axis = KbmAxis_Tab; break;
	case GLFW_KEY_F1:axis = KbmAxis_F1; break;
	case GLFW_KEY_F2:axis = KbmAxis_F2; break;
	case GLFW_KEY_F3:axis = KbmAxis_F3; break;
	case GLFW_KEY_F4:axis = KbmAxis_F4; break;
	case GLFW_KEY_F5:axis = KbmAxis_F5; break;
	case GLFW_KEY_F6:axis = KbmAxis_F6; break;
	case GLFW_KEY_F7:axis = KbmAxis_F7; break;
	case GLFW_KEY_F8:axis = KbmAxis_F8; break;
	case GLFW_KEY_F9:axis = KbmAxis_F9; break;
	case GLFW_KEY_F10:axis = KbmAxis_F10; break;
	case GLFW_KEY_F11:axis = KbmAxis_F11; break;
	case GLFW_KEY_F12:axis = KbmAxis_F12; break;
	case GLFW_KEY_F13:axis = KbmAxis_F13; break;
	case GLFW_KEY_F14:axis = KbmAxis_F14; break;
	case GLFW_KEY_F15:axis = KbmAxis_F15; break;
		/* GLFW_KEY_F16 => 25 */
	case GLFW_KEY_KP_0:axis = KbmAxis_Pad0; break;
	case GLFW_KEY_KP_1:axis = KbmAxis_Pad1; break;
	case GLFW_KEY_KP_2:axis = KbmAxis_Pad2; break;
	case GLFW_KEY_KP_3:axis = KbmAxis_Pad3; break;
	case GLFW_KEY_KP_4:axis = KbmAxis_Pad4; break;
	case GLFW_KEY_KP_5:axis = KbmAxis_Pad5; break;
	case GLFW_KEY_KP_6:axis = KbmAxis_Pad6; break;
	case GLFW_KEY_KP_7:axis = KbmAxis_Pad7; break;
	case GLFW_KEY_KP_8:axis = KbmAxis_Pad8; break;
	case GLFW_KEY_KP_9:axis = KbmAxis_Pad9; break;
		//case GLFW_KEY_KP_DECIMAL:axis = KbmAxis_End; break;
	case GLFW_KEY_KP_DIVIDE:axis = KbmAxis_PadDivide; break;
	case GLFW_KEY_KP_MULTIPLY:axis = KbmAxis_PadMultiply; break;
	case GLFW_KEY_KP_SUBTRACT:axis = KbmAxis_PadMinus; break;
	case GLFW_KEY_KP_ADD:axis = KbmAxis_PadPlus; break;
	case GLFW_KEY_KP_ENTER:axis = KbmAxis_PadEnter; break;
	case GLFW_KEY_KP_EQUAL:axis = KbmAxis_PadEquals; break;
	case GLFW_KEY_LEFT_SHIFT:axis = KbmAxis_LeftShift; break;
	case GLFW_KEY_LEFT_CONTROL:axis = KbmAxis_LeftControl; break;
	case GLFW_KEY_LEFT_ALT:axis = KbmAxis_LeftAlt; break;
	case GLFW_KEY_LEFT_SUPER:axis = KbmAxis_LeftCommand; break;
	case GLFW_KEY_RIGHT_SHIFT:axis = KbmAxis_RightShift; break;
	case GLFW_KEY_RIGHT_CONTROL:axis = KbmAxis_RightControl; break;
	case GLFW_KEY_RIGHT_ALT:axis = KbmAxis_RightAlt; break;
	case GLFW_KEY_RIGHT_SUPER:axis = KbmAxis_RightCommand; break;
		//case GLFW_KEY_MENU:axis = KbmAxis_RightCommand; break;
		//case GLFW_KEY_LAST:axis = KbmAxis_RightCommand; break;

	default:
		axis = KbmAxis_None;
		break;
	}
	return axis;
}

KbmAxis KbmDevice::modAxisFromGLFW(int keycode)
{
	KbmAxis axis;
	switch (keycode)
	{
	case GLFW_KEY_LEFT_SHIFT:
	case GLFW_KEY_RIGHT_SHIFT:axis = KbmAxis_Shift; break;
	case GLFW_KEY_LEFT_CONTROL:
	case GLFW_KEY_RIGHT_CONTROL:axis = KbmAxis_Control; break;
	case GLFW_KEY_LEFT_ALT:
	case GLFW_KEY_RIGHT_ALT:axis = KbmAxis_Alt; break;
	case GLFW_KEY_LEFT_SUPER:
	case GLFW_KEY_RIGHT_SUPER:axis = KbmAxis_Command; break;
	default:
		axis = KbmAxis_None;
		break;
	}
	return axis;
}

KbmAxis KbmDevice::mouseAxisFromGLFW(int mouseButton)
{
	KbmAxis axis;
	switch (mouseButton)
	{
	case GLFW_MOUSE_BUTTON_1:axis = KbmAxis_MouseLeft; break;
	case GLFW_MOUSE_BUTTON_2:axis = KbmAxis_MouseRight; break;
	case GLFW_MOUSE_BUTTON_3:axis = KbmAxis_MouseMiddle; break;
	case GLFW_MOUSE_BUTTON_4:axis = KbmAxis_MouseButton4; break;
	case GLFW_MOUSE_BUTTON_5:axis = KbmAxis_MouseButton5; break;
	case GLFW_MOUSE_BUTTON_6:axis = KbmAxis_MouseButton6; break;
	case GLFW_MOUSE_BUTTON_7:axis = KbmAxis_MouseButton7; break;
	case GLFW_MOUSE_BUTTON_8:axis = KbmAxis_MouseButton8; break;
	default:
		axis = KbmAxis_None;
		break;
	}
	return KbmAxis();
}