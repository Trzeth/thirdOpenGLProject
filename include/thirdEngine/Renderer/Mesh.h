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
	Mesh(const Mesh& mesh);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, Material material);
	~Mesh();

	virtual void operator=(const Mesh& mesh);

	bool operator==(const Mesh& m) const
	{
		return m.name == name;
	}

	virtual void GenVAO();

	//Remember To GenVAO
	virtual void Draw();

	std::string name;
protected:
	std::unique_ptr<MeshImpl> impl;

	Material material;
};

namespace std
{
	template<> struct hash<Mesh>
	{
		std::size_t operator()(const Mesh& m) const noexcept
		{
			return  size_t(0);
		}
	};
}