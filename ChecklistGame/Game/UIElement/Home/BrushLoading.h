#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Renderer/Texture.h>

class BrushLoading :public UIElement
{
public:
	BrushLoading(EventManager& eventManager);
	void Update(float dt);
	void Draw();
	void Show();
	bool IsVisible() { return isVisible; };
private:
	bool isVisible;
	bool hasSend;

	EventManager& eventManager;

	Texture brushLoadingTexture;

	double curTime;
	float curXPos;
};
