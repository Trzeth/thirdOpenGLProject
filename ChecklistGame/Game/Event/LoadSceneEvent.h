#pragma once
#include <thirdEngine/Framework/Event.h>

class LoadSceneEndEvent :public Event
{ };

class LoadSceneWaitEvent :public Event
{ };

class BikeMapChangeMapEvent :public Event
{
public:
	unsigned int dst;
};