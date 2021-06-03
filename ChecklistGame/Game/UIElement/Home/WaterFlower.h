#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Renderer/Texture.h>

class WaterFlower :public UIElement
{
public:
	WaterFlower(EventManager& eventManager);
	void Update(float dt);
	void Draw();
	void Show();
	bool IsVisible() { return isVisible; };
private:
	bool isVisible;
	EventManager& eventManager;

	Texture animationTexture[6];

	int curPos;
	double curTime;
	double interTime;
};
