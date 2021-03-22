#pragma once
#include "Mesh.h"

struct SkinnedMeshImpl;

class SkinnedMesh :public Mesh
{
public:
	SkinnedMesh();
	SkinnedMesh(const SkinnedMesh& mesh);
	SkinnedMesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<VertexBoneData> vertexBoneData, std::vector<BoneData> boneData, glm::mat4 transform, Material material);

	~SkinnedMesh();

	virtual void operator=(const SkinnedMesh& mesh);

	virtual void GenVAO();

	virtual void Draw();
protected:
	std::unique_ptr<SkinnedMeshImpl> skinnedImpl;
};
