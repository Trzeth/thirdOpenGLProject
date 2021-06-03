#pragma once
#include <stdio.h>

class UIElement
{
public:
	virtual ~UIElement() = default;
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual bool IsVisible() = 0;
};
