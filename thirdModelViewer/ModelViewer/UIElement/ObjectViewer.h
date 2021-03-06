#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>

class ObjectViewer :public UIElement
{
public:
	void Draw();
	void Update(float dt) { }
	bool IsVisible() { return true; }

	void SetTextureID(unsigned int id);
	float GetWindowWidth();
	float GetWindowHeight();
private:
	float windowWidth;
	float windowHeight;
	unsigned int textureID;
};
