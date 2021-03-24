#include <exception>
#include <glad/glad.h>

#include "Mesh.h"
#include "MeshImpl.h"
#include "Material.h"
#include "RenderUtil.h"

Mesh::Mesh() : impl(new MeshImpl()), material(), hasVertexBoneData(false)
{ }

Mesh::Mesh(const Mesh& mesh) : impl(new MeshImpl(*mesh.impl)), material(mesh.material), hasVertexBoneData(mesh.hasVertexBoneData)
{ }

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, Material material)
	: Mesh(vertices, indices, material, std::vector<VertexBoneData>(), std::vector<BoneData>())
{ }

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, Material material, std::vector<VertexBoneData> vertexBoneData, std::vector<BoneData> boneData)
	: Mesh()
{
	this->material = material;
	this->hasVertexBoneData = vertexBoneData.size() > 0 ? true : false;

	impl->nVertices = vertices.size();
	impl->nIndices = indices.size();
	impl->boneData = boneData;
	impl->boneTransforms.resize(impl->boneData.size());

	glGenBuffers(1, &impl->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, impl->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &impl->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, impl->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	if (this->hasVertexBoneData) {
		glGenBuffers(1, &impl->VBO_bone);
		glBindBuffer(GL_ARRAY_BUFFER, impl->VBO_bone);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBoneData) * vertexBoneData.size(), &vertexBoneData[0], GL_STATIC_DRAW);
	}
}

Mesh::~Mesh()
{ }

void Mesh::operator=(const Mesh& mesh)
{
	this->impl = std::unique_ptr<MeshImpl>(new MeshImpl(*mesh.impl));
	this->material = mesh.material;
}

void Mesh::GenVAO() const
{
	glGenVertexArrays(1, &impl->VAO);

	glBindVertexArray(impl->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, impl->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, impl->EBO);
	// 顶点位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// 顶点法线
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// 顶点纹理坐标
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	if (this->hasVertexBoneData) {
		glBindBuffer(GL_ARRAY_BUFFER, impl->VBO_bone);
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)offsetof(VertexBoneData, boneWeights));
	}

	glBindVertexArray(0);

	glCheckError();
}

void Mesh::Draw() const
{
	/*!
	 * @brief Don't forget to Gen VAO
	*/
	assert(impl->VAO != 0);

	glBindVertexArray(impl->VAO);
	glDrawElements(GL_TRIANGLES, impl->nIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

std::vector<glm::mat4> Mesh::GetBoneTransforms(const std::vector<glm::mat4>& nodeTransforms) const
{
	if (nodeTransforms.size() <= 0) {
		return impl->boneTransforms;
	}

	// Assume 0 is the root node
	glm::mat4 globalInverse = glm::inverse(nodeTransforms[0]);
	for (unsigned int i = 0; i < impl->boneData.size(); i++) {
		const BoneData& boneData = impl->boneData[i];
		glm::mat4 nodeTransform = nodeTransforms[boneData.nodeId];
		glm::mat4 boneOffset = boneData.boneOffset;
		glm::mat4 boneTransform = globalInverse * nodeTransform * boneOffset;
		impl->boneTransforms[i] = boneTransform;
	}

	return impl->boneTransforms;
}