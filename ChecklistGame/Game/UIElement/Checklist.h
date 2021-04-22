#pragma once
#include <thirdEngine/Renderer/UI/UIElement.h>
#include <thirdEngine/Framework/EventManager.h>
#include <thirdEngine/Input/Input.h>
#include <thirdEngine/Renderer/Texture.h>

#include "Game/GlobalVariable.h"

class Checklist :public UIElement
{
public:
	Checklist(Input& input, GlobalVariable& globalVariable);
	void Update(float dt) { };
	void Draw();
	bool IsVisible() { return isVisible; };
private:
	bool isVisible;
	Input& input;
	GlobalVariable& globalVariable;

	Texture checklistTexture;
};