#pragma once
#include <thirdEngine/Framework/Event.h>

class ClothStoreChangeClothSceneEvent :public Event
{
public:
	ClothStoreChangeClothSceneEvent(unsigned index) :clothIndex(index)
	{ }

	unsigned clothIndex;
};
