#pragma once
#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Renderer/Renderer.h>
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/DefaultComponentConstructor.h>

class CharacterShowcaseComponent :public Component
{
public:
	CharacterShowcaseComponent()
	{ };

	struct Data
	{
		Data() { }
	};

	Data data;
};

class CharacterShowcaseComponentConstructor :public DefaultComponentConstructor<CharacterShowcaseComponent> {
	using DefaultComponentConstructor<CharacterShowcaseComponent>::DefaultComponentConstructor;
};
