#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Input/Input.h>
#include <thirdEngine/Renderer/Texture.h>

class BikeMap :public UIElement
{
public:
	BikeMap(EventManager& eventManager, unsigned type);
	void Update(float dt) { };
	void Draw();
	void Show();
	bool IsVisible() { return isVisible; };
private:
	bool isVisible;
	EventManager& eventManager;
	unsigned type;
	Texture mapTexture;
};
