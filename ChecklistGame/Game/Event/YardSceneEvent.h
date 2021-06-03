#pragma once
#include <thirdEngine/Framework/Event.h>

class YardSceneLetterCloseEvent :public Event
{ };

class YardSceneOpenSceneStoryboardEndEvent :public Event
{ };

class YardSceneTurnAroundStoryboardEndEvent :public Event
{ };

class YardSceneBrushInteractEvent :public Event
{ };

class YardSceneBrushChangedEvent :public Event
{ };

class YardSceneBrushLoadingMidEvent :public Event
{ };

class YardSceneBrushFinishEvent :public Event
{ };

class YardSceneWaterPotInteractEvent :public Event
{ };

class YardSceneWaterPotFinishEvent :public Event
{ };

class YardSceneDoorInteractEvent :public Event
{ };

class YardSceneBikeInteractEvent :public Event
{ };
