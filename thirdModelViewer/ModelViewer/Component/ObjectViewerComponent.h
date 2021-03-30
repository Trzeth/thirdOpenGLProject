#pragma once
#include <vector>
#include <string>
#include <map>

#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/DefaultComponentConstructor.h>

#include <thirdEngine/Renderer/Shader.h>

enum class ViewerState
{
	FPSCamera,
	FixedCamera
};

struct ModelFileInfo {
	//UTF-8 Encoding
	std::string FullPath;
	std::string FileName;
	std::string DirPath;

	std::vector<std::string> AnimationNameList;
};

struct ShaderFileInfo {
	ShaderFileInfo(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath)
		:Name(name), VertexShaderPath(vertexShaderPath), FragmentShaderPath(fragmentShaderPath) { }
	std::string Name;
	std::string VertexShaderPath;
	std::string FragmentShaderPath;
};

class ObjectViewerComponent :public Component
{
public:
	ObjectViewerComponent()
		:viewerState(ViewerState::FPSCamera), fileInfo(), shaderList()
		, currentAnimationIndex(0), currentShaderIndex(0)
		, fileChangedFlag(false), animationChangedFlag(false), shaderChangedFlag(false), shaderReloadFlag(false)
	{  };

	struct Data
	{
		Data() :FPSCamera(World::NullEntity), FixedCamera(World::NullEntity) {};
		eid_t FPSCamera;
		eid_t FixedCamera;
	};

	Data data;

	bool fileChangedFlag;
	ModelFileInfo fileInfo;

	bool animationChangedFlag;
	int currentAnimationIndex;

	std::vector<std::pair<ShaderFileInfo, Shader>> shaderList;
	bool shaderChangedFlag;
	bool shaderReloadFlag;
	int currentShaderIndex;

	ViewerState viewerState;
};

class ObjectViewerComponentConstructor :public DefaultComponentConstructor<ObjectViewerComponent> {
	using DefaultComponentConstructor<ObjectViewerComponent>::DefaultComponentConstructor;
};
