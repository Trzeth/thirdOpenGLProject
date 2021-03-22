#pragma once
#include <glad/glad.h>

#include "SkinnedMesh.h"

struct SkinnedMeshImpl
{
	SkinnedMeshImpl() :VBO_bone(0) { };

	GLuint VBO_bone;

	std::vector<BoneData> boneData;
	std::vector<glm::mat4> boneTransforms;
};
