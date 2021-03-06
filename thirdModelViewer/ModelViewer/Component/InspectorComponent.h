#pragma once
#include <glad/glad.h>
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/ComponentConstructor.h>

#include <thirdEngine/Renderer/UI/UIRenderer.h>

#include "ModelViewer/Component/ObjectViewerComponent.h"

#include "ModelViewer/UIElement/ObjectViewer.h"
#include "ModelViewer/UIElement/Sidebar.h"

//ModelViewer UI Componet
struct InspectorComponent :public Component
{
	InspectorComponent() { }

	UIRenderer::UIElementHandle dockSpaceHandle;
	UIRenderer::UIElementHandle sidebarHandle;
	UIRenderer::UIElementHandle objectViewerHandle;

	//UIElemtn ObjectViewer
	std::shared_ptr<ObjectViewer> objectViewer;
	std::shared_ptr<Sidebar> sidebar;

	GLuint frameBuffer;
	GLuint texColorBuffer;
	//Depth Buffer
	GLuint renderBuffer;

	//Viewer Entity ID
	eid_t viewer;

	//Use to Check Window Resize
	float preWindowWidth;
	float preWindowHeight;
};

class InspectorComponentConstructor :public ComponentConstructor {
public:
	InspectorComponentConstructor(UIRenderer& uiRenderer)
		:uiRenderer(uiRenderer) { }

	virtual ComponentConstructorInfo Construct(World& world, eid_t parent, void* userinfo);
	virtual void Finish(World& world, eid_t entity);
private:
	UIRenderer& uiRenderer;
};
