#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Renderer/Texture.h>

class ItemInfo :public UIElement
{
public:
	ItemInfo();
	void Update(float dt) { };
	void Draw();
	void Show(unsigned index);
	bool IsVisible() { return isVisible; };
private:
	bool isVisible;

	std::vector<Texture> textures;

	unsigned index;
};
