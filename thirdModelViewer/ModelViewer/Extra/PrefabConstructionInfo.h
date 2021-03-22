#pragma once
#include "ModelViewer/Extra/Transform.h"

struct PrefabConstructionInfo
{
	PrefabConstructionInfo() { }
	PrefabConstructionInfo(const Transform& transform) : initialTransform(transform) { }
	Transform initialTransform;
};
