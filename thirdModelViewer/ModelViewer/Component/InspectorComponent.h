#pragma once
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/ComponentConstructor.h>

#include <thirdEngine/Renderer/UI/UIRenderer.h>

struct InspectorComponent :public Component
{
	InspectorComponent() :uiElementHandle(nullptr) { }

	UIRenderer::UIElementHandle uiElementHandle;
};

class InspectorComponentConstructor :public ComponentConstructor {
public:
	InspectorComponentConstructor(UIRenderer& uiRenderer)
		:uiRenderer(uiRenderer) { }

	virtual ComponentConstructorInfo Construct(World& world, eid_t parent, void* userinfo)const;
	virtual void Finish(World& world, eid_t entity);
private:
	UIRenderer& uiRenderer;
};
