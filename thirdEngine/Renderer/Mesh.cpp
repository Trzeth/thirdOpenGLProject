#include <exception>
#include <glad/glad.h>

#include "Mesh.h"
#include "MeshImpl.h"
#include "Material.h"
#include "RenderUtil.h"

Mesh::Mesh() : impl(new MeshImpl()), material(), hasVertexBoneData(false) { }

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::shared_ptr<Material> material)
	: Mesh(vertices, indices, material, std::vector<VertexBoneData>(), std::vector<BoneData>())
{ }

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::shared_ptr<Material> material, std::vector<VertexBoneData> vertexBoneData, std::vector<BoneData> boneData)
{
	impl = std::make_unique<MeshImpl>();

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	if (this->hasVertexBoneData) {
		glGenBuffers(1, &impl->VBO_bone);
		glBindBuffer(GL_ARRAY_BUFFER, impl->VBO_bone);
		glBufferData(GL_ARRAY_BUFFER, vertexBoneData.size() * sizeof(VertexBoneData), &vertexBoneData[0], GL_STATIC_DRAW);
	}

	glCheckError();
}

Mesh::~Mesh()
{
	if (!impl)
		return;

	glDeleteBuffers(1, &impl->VBO);
	glDeleteBuffers(1, &impl->EBO);

	if (this->hasVertexBoneData)
	{
		glDeleteBuffers(1, &impl->VBO_bone);
	}

	glDeleteVertexArrays(1, &impl->VAO);
}

Mesh::Mesh(const Mesh& other)
{
	*this = other;
}

Mesh& Mesh::operator=(const Mesh& other)
{
#ifdef _DEBUG
	printf("Mesh Copy\n");
#endif // _DEBUG

	impl = std::make_unique<MeshImpl>();
	hasVertexBoneData = other.hasVertexBoneData;
	name = other.name;

	glGenBuffers(1, &impl->VBO);
	glBindBuffer(GL_COPY_READ_BUFFER, other.impl->VBO);
	glBindBuffer(GL_COPY_WRITE_BUFFER, impl->VBO);

	GLint size = 0;
	glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
	glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

	glGenBuffers(1, &impl->EBO);
	glBindBuffer(GL_COPY_READ_BUFFER, other.impl->EBO);
	glBindBuffer(GL_COPY_WRITE_BUFFER, impl->EBO);

	glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
	glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

	if (other.hasVertexBoneData) {
		glGenBuffers(1, &impl->VBO_bone);
		glBindBuffer(GL_COPY_READ_BUFFER, other.impl->VBO_bone);
		glBindBuffer(GL_COPY_WRITE_BUFFER, impl->VBO_bone);

		glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
		glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
	}

	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (other.impl->VAO) {
		this->GenVAO();
		glCheckError();
	}

	this->material = other.material;
	glCheckError();

	return *this;
}

Mesh::Mesh(Mesh&& other) noexcept = default;

Mesh& Mesh::operator=(Mesh && other) noexcept = default;

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

std::vector<glm::mat4> Mesh::GetBoneTransforms(const std::vector<glm::mat4>&nodeTransforms) const
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