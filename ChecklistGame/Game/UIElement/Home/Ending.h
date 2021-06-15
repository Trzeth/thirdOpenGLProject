#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Renderer/Texture.h>

class Ending :public UIElement
{
public:
	Ending();
	void Update(float dt);
	void Draw();
	bool IsVisible() { return isVisible; };
private:
	bool isVisible;

	std::vector<Texture> textures;

	int curPos;
	double curTime;
	double interTime;
};
