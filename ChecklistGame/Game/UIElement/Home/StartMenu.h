#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Renderer/Texture.h>

class StartMenu :public UIElement
{
public:
	StartMenu(EventManager& eventManager);
	void Update(float dt) { };
	void Draw();
	bool IsVisible() { return isVisible; };
private:
	EventManager& eventManager;

	bool isVisible;

	Texture titleTexture;
	Texture playBtnTexture;
	Texture playBtnHoverTexture;
	Texture exitBtnTexture;
	Texture exitBtnHoverTexture;
};
