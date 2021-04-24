#pragma once
#include <thirdEngine/Framework/Event.h>

class LoadSceneStartEvent :public Event
{ };

class LoadSceneEndEvent :public Event
{ };

class LoadSceneWaitEvent :public Event
{ };