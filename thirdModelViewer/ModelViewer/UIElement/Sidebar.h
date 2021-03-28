#pragma once
#include <string>
#include <vector>
#include <thirdEngine/Renderer/UI/UIElement.h>

class Sidebar :public UIElement
{
public:
	Sidebar();

	void Draw();
	void Update(float dt) { }

	int curCameraType;
	float cameraSpeed;
	bool resetCamera;
	bool playAnim;
	bool pauseAnim;

	bool modelChangedFlag;
	std::string modelPath;
	std::string fileName;
	std::string path;

	bool animNameChangedFlag;
	std::string curAnimName;
	std::vector<std::string> animNames;

	std::vector<std::string> material;
};
