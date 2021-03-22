#pragma once
#include <glm/glm.hpp>
#include <tsl/ordered_map.h>

#include <vector>
#include <map>
#include <memory>

#include "Mesh.h"
#include "Material.h"
#include "Animation.h"

enum class ModelType {
	Normal,
	Skinned
};

/*! Node in the model's hierarchy. This is strongly tied to an AnimationData struct.
	An "index" here refers to an index into AnimationData.nodes. */
struct ModelNode
{
	/*! Name of this node, purely for debugging purposes. */
	std::string name;

	/*! Transform of this node. */
	glm::mat4 transform;

	/*! True if the node is the root node. */
	bool isRoot;

	/*! Index of this node's parent. */
	unsigned int parent;

	/*! Indices of this node's children. */
	std::vector<unsigned int> children;
};

class Model
{
public:
	friend class ModelLoader;
	friend class Renderer;

	Model();

	std::vector<glm::mat4> GetNodeTransforms(const std::string& animation, float time, AnimationContext& context);

	//virtual void Draw();

	ModelType Type() { return type; }
protected:
	ModelType type;
	// Transform Model to Normal Position (From 3DSMAX TO GAME)
	// transformToOrigin
	glm::mat4 transform;

	/*! The node hierarchy. */
	std::vector<ModelNode> nodes;

	/*! A map of node names to node IDs. (Is this necessary?) */
	std::unordered_map<std::string, unsigned int> nodeIdMap;

	// Identify by Node name
	tsl::ordered_map<Mesh, std::map<std::string, glm::mat4>> meshes;

	/*! The animations themselves. */
	std::unordered_map<std::string, Animation> animations;

	/*! Cached transforms returned from getNodeTransforms. */
	std::vector<glm::mat4> nodeTransforms;
};
