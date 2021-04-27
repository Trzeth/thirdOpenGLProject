#pragma once
#include <glm/glm.hpp>

#include <vector>
#include <memory>

#include "Material.h"

constexpr int MAX_BONES_PER_VERTEX = 4;

struct Vertex
{
	Vertex() :position(0.0f), normal(0.0f), texCoords(0.0), tintColor(1.0f) { };

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tintColor;
};

struct VertexBoneData {
	VertexBoneData() :boneIds{ 0 }, boneWeights{ 0 } { }

	unsigned int boneIds[MAX_BONES_PER_VERTEX];
	float boneWeights[MAX_BONES_PER_VERTEX];

	void addWeight(unsigned id, float weight) {
		for (int i = 0; i != MAX_BONES_PER_VERTEX; i++) {
			if (boneWeights[i] == 0) {
				boneIds[i] = id;
				boneWeights[i] = weight;
				return;
			}
		}

		printf("WARNING: More than %d bones\n", MAX_BONES_PER_VERTEX);
	}
};

struct BoneData {
	// Mesh space to Local bone space
	glm::mat4 boneOffset;
	unsigned int nodeId;
};

struct MeshImpl;

class Mesh
{
public:
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, Material&& material);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, Material&& material, std::vector<VertexBoneData> vertexBoneData, std::vector<BoneData> boneData);
	~Mesh();

	Mesh(const Mesh& other);
	Mesh& operator=(const Mesh& other);

	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	void GenVAO() const;

	//Remember To GenVAO
	void Draw() const;

	/*!
	 * @brief Gets the transforms of each bone in boneData given the position
	 * @param nodeTransforms  The transforms of all of the nodes in the model.
	 * @return Matrices for each bone which transform from the bone's bind-pose space to the bone's new space (given by nodeTransforms[bone.nodeId]).
	 * The values in the returned vector directly correspond to the bones in this mesh's boneData vector.
	*/
	std::vector<glm::mat4> GetBoneTransforms(const std::vector<glm::mat4>& nodeTransforms) const;

	std::string name;
	Material material;
	std::unique_ptr<MeshImpl> impl;
	bool hasVertexBoneData;
};
