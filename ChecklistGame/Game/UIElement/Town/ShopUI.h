#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Renderer/Texture.h>

class ShopUI :public UIElement
{
public:
	ShopUI(EventManager& eventManager);
	void Update(float dt) { };
	void Draw();
	bool IsVisible() { return isVisible; };
private:
	bool isVisible;
	EventManager& eventManager;

	std::vector<bool> buttonStatus;

	Texture npcTexture;
	Texture butterTexture;
	Texture cheeseTexture;
	Texture meatTexture;
	Texture riceTexture;
};
