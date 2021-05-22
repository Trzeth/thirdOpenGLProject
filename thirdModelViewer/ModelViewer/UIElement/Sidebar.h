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
	bool IsVisible() { return true; }

private:
	void drawMetricsWindow();
	void drawGeneralWindow();
	void drawMaterialWindow();

	void drawMaterialProperties(std::map<std::string, MaterialProperty>& properties);

	ObjectViewerComponent* objectViewerComponent;
};
