#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Renderer/Texture.h>
#include <vector>

class DeerNPCConversation : public UIElement
{
public:
	DeerNPCConversation(EventManager& eventManager);
	void Update(float dt) { };
	void Draw();
	void Show();
	bool IsVisible() { return isVisible; };
private:
	bool isVisible;
	EventManager& eventManager;

	int index;
	std::vector<Texture> textures;
};
