#pragma once
#include <string>
#include <vector>

#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Renderer/UI/UIElement.h>

#include "ModelViewer/Component/ObjectViewerComponent.h"

class Sidebar :public UIElement
{
public:
	Sidebar(ObjectViewerComponent* objectViewerComponent);

	void Draw();
	void Update(float dt) { }

private:
	void DrawMetricsWindow();
	void DrawGeneralWindow();
	void DrawMaterialWindow();

	ObjectViewerComponent* objectViewerComponent;
};
