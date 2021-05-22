#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>

class DockSpace :public UIElement
{
public:
	void Draw();

	void Update(float dt) { };
	bool IsVisible() { return true; }
};
