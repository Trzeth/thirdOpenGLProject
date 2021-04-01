#pragma once
#include <vector>
#include <string>
#include <map>

#include <thirdEngine/Framework/World.h>
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/DefaultComponentConstructor.h>
#include <thirdEngine/Renderer/Shader.h>

#include "ModelViewer/Material/MaterialWrapper.h"

enum class ViewerState
{
	FPSCamera,
	FixedCamera
};

struct ModelFileInfo {
	ModelFileInfo()
		:FullPath(), FileName(), DirPath(), AnimationNameList()
	{ }
	//UTF-8 Encoding
	std::string FullPath;
	std::string FileName;
	std::string DirPath;

	std::vector<std::string> AnimationNameList;
};

class ObjectViewerComponent :public Component
{
public:
	ObjectViewerComponent()
		:FPSCamera(World::NullEntity), FixedCamera(World::NullEntity),
		viewerState(ViewerState::FPSCamera), fileInfo(), materialList()
		, currentAnimationIndex(0), currentMaterialIndex(0)
		, fileChangedFlag(false), animationChangedFlag(false), materialChangedFlag(false), materialReloadFlag(false)
	{  };

	bool fileChangedFlag;
	ModelFileInfo fileInfo;

	bool animationChangedFlag;
	int currentAnimationIndex;

	std::vector<MaterialWrapper> materialList;
	bool materialChangedFlag;
	bool materialReloadFlag;
	int currentMaterialIndex;

	eid_t FPSCamera;
	eid_t FixedCamera;

	ViewerState viewerState;
};

class ObjectViewerComponentConstructor :public ComponentConstructor {
public:
	ObjectViewerComponentConstructor(std::vector<MaterialWrapper> materialList)
		:materialList(materialList) { }

	virtual ComponentConstructorInfo Construct(World& world, eid_t parent, void* userinfo)const;
	virtual void Finish(World& world, eid_t entity);
private:
	std::vector<MaterialWrapper> materialList;
};
