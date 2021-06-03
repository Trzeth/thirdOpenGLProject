#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Renderer/Texture.h>

class BrushSelector :public UIElement
{
public:
	BrushSelector(EventManager& eventManager);
	void Update(float dt) { };
	void Draw();
	void Show();
	bool IsVisible() { return isVisible; };

	unsigned int GetCurrentSelected() const;
private:
	bool isVisible;
	EventManager& eventManager;

	Texture brushTexture[4];

	Texture closeBtnTexture;
	Texture closeBtnHoverTexture;

	int curBrushSelected = 0;
};