#include <unordered_map>
#include <cmath>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ModelLoader.h"
#include "Mesh.h"
#include "Texture.h"
#include "TextureImpl.h"

struct ModelLoader::Impl
{
	/*! The relative directory to the model we are currently loading. */
	std::string curDir;

	/*! Cache mapping IDs to models. The IDs can also be paths. */
	std::unordered_map<std::string, Model> modelIdCache;

	/*! Cache mapping IDs to textures. The IDs are usually paths. */
	std::unordered_map<std::string, TextureImpl> textureCache;

	/*! Used to load textures from imported models. */
	TextureLoader textureLoader;

	/*! Default material properties.*/
	std::shared_ptr<Material> defaultMaterial = std::make_shared<Material>();

	/*!
	 * \brief Processes an assimp model, starting from its root node.
	 */
	Model processModel(aiNode* node, const aiScene* scene);

	/*!
	 * @brief Process materials
	 * @param scene
	 * @return
	*/
	std::vector<Material> processMaterials(const aiScene* scene);

	/*!
	 * @brief Processes meshes of a model.
	 * @param nodeIdMap A map of node names to internal node IDs. Used when the bones are being loaded from the mesh.
	 */
	std::vector<Mesh> processMeshes(const aiScene* scene, std::unordered_map<std::string, unsigned int> nodeIdMap, const std::vector<Material>& materials);

	/*!
	 * @brief Process a single mesh
	 * @return mesh
	*/
	Mesh processMesh(aiMesh* mesh, const aiScene* scene, std::unordered_map<std::string, unsigned int> nodeIdMap, const std::vector<Material>& materials);

	AnimationData processAnimations(const aiScene* scene, const std::unordered_map<std::string, unsigned int>& nodeIdMap);

	/*!
	 * \brief Processes and returns the bone data of a specific mesh.
	 * \param nodeIdMap Map of node names to internal node IDs.
	 * \param vertexBoneData Output parameter containing bone (node) IDs and the corresponding weights. The length
	 *		is equal to mesh->mNumVertices.
	 * \param boneData The data loaded from each of the bones.
	 */
	void loadBoneData(aiMesh* mesh, const aiScene* scene, std::unordered_map<std::string, unsigned int> nodeIdMap, std::vector<VertexBoneData>& vertexBoneData, std::vector<BoneData>& boneData);

	/*!
	 * \brief Loads textures from a material, but only of a specific type.
	 */
	std::vector<Texture> loadMaterialTextures(const std::string& relDir, aiMaterial* mat, aiTextureType type);

	// Utility functions
	glm::vec3 aiToGlm(aiVector3D vec3) { return glm::vec3(vec3.x, vec3.y, vec3.z); };
	glm::quat aiToGlm(aiQuaternion quat) { return glm::quat(aiToGlm(quat.GetMatrix())); };
	glm::mat4 aiToGlm(aiMatrix4x4 mat4) { return glm::transpose(glm::make_mat4(&mat4.a1)); };
	glm::mat3 aiToGlm(aiMatrix3x3 mat3) { return glm::transpose(glm::make_mat3(&mat3.a1)); };
};

ModelLoader::ModelLoader()
	:impl(new Impl())
{ }

ModelLoader::~ModelLoader()
{
}

Model ModelLoader::LoadModel(const std::string& path)
{
	auto modelCacheIter = this->impl->modelIdCache.find(path);
	if (modelCacheIter != this->impl->modelIdCache.end()) {
		return modelCacheIter->second;
	}
	else {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			printf("Assimp error while loading model: %s\n", importer.GetErrorString());
			return Model();
		}

		this->impl->curDir = path.substr(0, path.find_last_of('/') + 1);
		Model model = this->impl->processModel(scene->mRootNode, scene);
		model.material = impl->defaultMaterial;
		this->impl->modelIdCache.emplace(std::make_pair(path, model));
		return model;
	}
}

void ModelLoader::SetDefaultMaterial(const std::shared_ptr<Material>& material)
{
	impl->defaultMaterial = material;
}

Model ModelLoader::Impl::processModel(aiNode* rootNode, const aiScene* scene)
{
	Model model;
	std::vector<Material> material = processMaterials(scene);

	std::vector<ModelNode> nodeHierarchy;
	std::unordered_map<std::string, unsigned int> nodeIdMap;

	std::vector<std::vector<glm::mat4>> meshesTransform(scene->mNumMeshes);
	std::vector<std::vector<int>> meshNodeId(scene->mNumMeshes);

	bool hasAnimation = scene->mNumAnimations > 0 ? true : false;
	std::vector<glm::mat4> bindposeTransform;

	//Process STACK
	std::vector<aiNode*> processQueue;
	std::vector<glm::mat4> cachedTransform;

	processQueue.push_back(rootNode);
	cachedTransform.push_back(glm::mat4(1.0f));

	while (processQueue.size() > 0) {
		aiNode* ai_node = processQueue.back();
		processQueue.pop_back();
		std::string nodeName(ai_node->mName.data);

		unsigned int nodeId = nodeHierarchy.size();
		nodeIdMap[nodeName] = nodeId;

		ModelNode node;
		node.name = nodeName;
		node.transform = aiToGlm(ai_node->mTransformation);
		if (ai_node->mParent != NULL) {
			auto iter = nodeIdMap.find(ai_node->mParent->mName.data);
			assert(iter != nodeIdMap.end());
			// Assign parent
			node.parent = iter->second;
			// Assign self to parent's children
			nodeHierarchy[iter->second].children.push_back(nodeId);
			node.isRoot = false;
		}
		else {
			node.isRoot = true;
		}

		glm::mat4 globalTransform = cachedTransform.back() * aiToGlm(ai_node->mTransformation);
		cachedTransform.pop_back();

		for (unsigned int i = 0; i != ai_node->mNumMeshes; i++) {
			meshesTransform[ai_node->mMeshes[i]].push_back(globalTransform);

			if (hasAnimation)meshNodeId[ai_node->mMeshes[i]].push_back(nodeHierarchy.size());
		}

		if (hasAnimation)bindposeTransform.push_back(globalTransform);

		nodeHierarchy.push_back(node);

		for (unsigned int i = 0; i < ai_node->mNumChildren; i++) {
			processQueue.push_back(ai_node->mChildren[i]);
			cachedTransform.push_back(globalTransform);
		}
	}

	model.meshes = processMeshes(scene, nodeIdMap, material);
	model.meshesTransform = meshesTransform;
	model.cachedNodeTransforms.resize(nodeHierarchy.size());

	if (hasAnimation)
	{
		model.animationData = processAnimations(scene, nodeIdMap);
		model.animationData.nodes = nodeHierarchy;
		model.animationData.meshNodeId = meshNodeId;
		model.animationData.bindposeNodeTransforms = bindposeTransform;
	}

	return model;
}

std::vector<Mesh> ModelLoader::Impl::processMeshes(const aiScene* scene, std::unordered_map<std::string, unsigned int> nodeIdMap, const std::vector<Material>& materials)
{
	std::vector<Mesh> meshes;

	for (int i = 0; i != scene->mNumMeshes; i++) {
		meshes.push_back(processMesh(scene->mMeshes[i], scene, nodeIdMap, materials));
	}

	return meshes;
}

Mesh ModelLoader::Impl::processMesh(aiMesh* mesh, const aiScene* scene, std::unordered_map<std::string, unsigned int> nodeIdMap, const std::vector<Material>& materials)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (unsigned i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0]) {
			vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Material material = mesh->mMaterialIndex >= 0 ? materials[mesh->mMaterialIndex] : *defaultMaterial;

	std::vector<VertexBoneData> vertexBoneData;
	std::vector<BoneData> boneData;
	loadBoneData(mesh, scene, nodeIdMap, vertexBoneData, boneData);

	Mesh processedMesh(vertices, indices, material, vertexBoneData, boneData);

	return processedMesh;
}

void ModelLoader::Impl::loadBoneData(aiMesh* mesh, const aiScene* scene, std::unordered_map<std::string, unsigned int> nodeIdMap, std::vector<VertexBoneData>& vertexBoneData, std::vector<BoneData>& boneData)
{
	if (mesh->mNumBones > 0) {
		boneData.resize(mesh->mNumBones);
		vertexBoneData.resize(mesh->mNumVertices);
	}

	for (unsigned int i = 0; i != mesh->mNumBones; i++) {
		aiBone* bone = mesh->mBones[i];
		std::string boneName(bone->mName.data);

		auto iter = nodeIdMap.find(boneName);
		assert(iter != nodeIdMap.end());
		boneData[i].nodeId = iter->second;
		boneData[i].boneOffset = aiToGlm(bone->mOffsetMatrix);

		for (unsigned int j = 0; j < bone->mNumWeights; j++) {
			aiVertexWeight weight = bone->mWeights[j];
			vertexBoneData[weight.mVertexId].addWeight(i, weight.mWeight);
		}
	}
}

std::vector<Material> ModelLoader::Impl::processMaterials(const aiScene* scene)
{
	std::vector<Material> materials;

	for (int i = 0; i != scene->mNumMaterials; i++) {
		std::vector<Texture> textures;

		{
			aiMaterial* material = scene->mMaterials[i];
			std::vector<Texture> diffuseMaps = this->loadMaterialTextures(this->curDir, material, aiTextureType_DIFFUSE);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<Texture> specularMaps = this->loadMaterialTextures(this->curDir, material, aiTextureType_SPECULAR);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			if (false)// TODO: Map assimp properties to our properties
				for (unsigned i = 0; i < material->mNumProperties; i++) {
					aiMaterialProperty* materialProperty = material->mProperties[i];
				}
		}

		Material material = *defaultMaterial;
		material.SetTextures(textures);
		materials.emplace_back(material);
	}

	return materials;
}

std::vector<Texture> ModelLoader::Impl::loadMaterialTextures(const std::string& relDir, aiMaterial* mat, aiTextureType type)
{
	std::vector<Texture> textures;
	for (unsigned i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path(str.C_Str());
		path = relDir + path;

		Texture texture;
		auto cacheIter = this->textureCache.find(path);
		if (cacheIter == this->textureCache.end()) {
			TextureType newType = (type == aiTextureType_DIFFUSE ? TextureType::Diffuse : TextureType::Specular);
			texture = textureLoader.LoadFromFile(newType, path);
			this->textureCache.emplace(std::make_pair(path, *texture.impl));
		}
		else {
			texture.impl = std::make_unique<TextureImpl>(cacheIter->second);
		}

		textures.emplace_back(texture);
	}
	return textures;
}

AnimationData ModelLoader::Impl::processAnimations(const aiScene* scene, const std::unordered_map<std::string, unsigned int>& nodeIdMap)
{
	AnimationData animationData;
	animationData.nodeIdMap = nodeIdMap;

	for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
		aiAnimation* ai_animation = scene->mAnimations[i];

		std::string animName(ai_animation->mName.data);

		// 3DSMAX FBX only export one animation
		if (scene->mNumAnimations == 1 && animName.empty()) {
			animName = "combinedAnim";
		}

		Animation& animation = animationData.animations[animName];

		float minTime = FLT_MAX;
		float maxTime = FLT_MIN;
		for (unsigned int j = 0; j < ai_animation->mNumChannels; j++) {
			aiNodeAnim* ai_channel = ai_animation->mChannels[j];

			std::string nodeName = ai_channel->mNodeName.data;

			auto iter = nodeIdMap.find(nodeName);
			assert(iter != nodeIdMap.end());

			Channel channel;
			channel.nodeId = iter->second;
			channel.positionKeys.resize(ai_channel->mNumPositionKeys);
			channel.rotationKeys.resize(ai_channel->mNumRotationKeys);
			channel.scaleKeys.resize(ai_channel->mNumScalingKeys);

			for (unsigned int k = 0; k < ai_channel->mNumPositionKeys; k++) {
				aiVectorKey ai_posKey = ai_channel->mPositionKeys[k];
				channel.positionKeys[k].time = (float)(ai_posKey.mTime / ai_animation->mTicksPerSecond);
				channel.positionKeys[k].value = aiToGlm(ai_posKey.mValue);
				if (ai_channel->mNumPositionKeys > 1) {
					// If there's only one keyframe, ignore it for time calculation purposes
					minTime = std::fmin(minTime, (float)ai_channel->mPositionKeys[k].mTime);
					maxTime = std::fmax(maxTime, (float)ai_channel->mPositionKeys[k].mTime);
				}
			}
			for (unsigned int k = 0; k < ai_channel->mNumRotationKeys; k++) {
				aiQuatKey ai_rotKey = ai_channel->mRotationKeys[k];
				channel.rotationKeys[k].time = (float)(ai_rotKey.mTime / ai_animation->mTicksPerSecond);
				channel.rotationKeys[k].value = aiToGlm(ai_rotKey.mValue);
				if (ai_channel->mNumRotationKeys > 1) {
					// If there's only one keyframe, ignore it for time calculation purposes
					minTime = std::fmin(minTime, (float)ai_channel->mRotationKeys[k].mTime);
					maxTime = std::fmax(maxTime, (float)ai_channel->mRotationKeys[k].mTime);
				}
			}
			for (unsigned int k = 0; k < ai_channel->mNumScalingKeys; k++) {
				aiVectorKey ai_scaleKey = ai_channel->mScalingKeys[k];
				channel.scaleKeys[k].time = (float)(ai_scaleKey.mTime / ai_animation->mTicksPerSecond);
				channel.scaleKeys[k].value = aiToGlm(ai_scaleKey.mValue);
				if (ai_channel->mNumScalingKeys > 1) {
					// If there's only one keyframe, ignore it for time calculation purposes
					minTime = std::fmin(minTime, (float)ai_channel->mScalingKeys[k].mTime);
					maxTime = std::fmax(maxTime, (float)ai_channel->mScalingKeys[k].mTime);
				}
			}

			// Default to 30 ticks per second if unspecified
			float ticksPerSecond = (ai_animation->mTicksPerSecond != 0 ? (float)ai_animation->mTicksPerSecond : 1 / 30.0f);

			// In most cases, ignore ai_animation->mDuration - it seems to be inaccurate for
			// animations that don't start at time 0
			if (minTime != FLT_MIN) {
				animation.startTime = (float)(minTime / ticksPerSecond);
				animation.endTime = (float)(maxTime / ticksPerSecond);
			}
			else {
				animation.startTime = 0.0f;
				animation.endTime = (float)(ai_animation->mDuration / ticksPerSecond);
			}

			animation.channelIdMap[channel.nodeId] = animation.channels.size();
			animation.channels.push_back(channel);
		}
	}

	return animationData;
}