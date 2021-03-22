#include <glad/glad.h>

#include "Mesh.h"
#include "MeshImpl.h"
#include "Material.h"

Mesh::Mesh() : impl(new MeshImpl()), material(), hasVertexBoneData(false)
{ }

Mesh::Mesh(const Mesh& mesh) : impl(new MeshImpl(*mesh.impl)), material(mesh.material), hasVertexBoneData(false)
{ }

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, Material material) : Mesh(vertices, indices, std::vector<VertexBoneData>(), std::vector<BoneData>())
{ }

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<VertexBoneData> vertexBoneData, std::vector<BoneData> boneData)
{
	this->material = material;
	hasVertexBoneData = vertexBoneData.size() > 0 ? true : false;

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

	if (hasVertexBoneData) {
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

void Mesh::GenVAO()
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

	if (hasVertexBoneData) {
		glBindBuffer(GL_ARRAY_BUFFER, impl->VBO_bone);
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)offsetof(VertexBoneData, boneIds));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)offsetof(VertexBoneData, boneWeights));
		glBindVertexArray(0);
	}

	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glBindVertexArray(impl->VAO);
	glDrawElements(GL_TRIANGLES, impl->nIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}