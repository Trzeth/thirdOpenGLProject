#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>
#include <thirdEngine/Renderer/Texture.h>

class Letter :public UIElement
{
public:
	Letter();
	void Update(float dt) { };
	void Draw();
	void Show();
	bool IsVisible() { return isVisible; };
private:
	bool isVisible;

	Texture letterTexture;
};
