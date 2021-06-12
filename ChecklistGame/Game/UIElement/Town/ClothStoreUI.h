#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Renderer/Texture.h>

class ClothStoreUI :public UIElement
{
public:
	ClothStoreUI(EventManager& eventManager);
	void Update(float dt) { };
	void Draw();
	bool IsVisible() { return isVisible; };
private:
	bool isVisible;
	EventManager& eventManager;
};
