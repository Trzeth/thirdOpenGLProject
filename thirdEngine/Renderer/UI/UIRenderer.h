#pragma once
#include "../HandlePool.h"
#include "UIElement.h"

#include <memory>
#include <list>

class UIElement;
struct GLFWwindow;

class UIRenderer
{
public:
	struct Entity {
		std::shared_ptr<UIElement> element;
	};

	typedef HandlePool<Entity>::Handle UIElementHandle;

	UIRenderer();

	void Initialize(GLFWwindow* window);

	UIElementHandle GetEntityHandle(const std::shared_ptr<UIElement>& element);

	void Update(float dt);
	void Draw();

private:
	HandlePool<UIRenderer::Entity> entityPool;
};
