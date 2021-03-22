#pragma once
#include <glad/glad.h>

struct MeshImpl
{
	MeshImpl() : VAO(0), VBO(0), EBO(0), nVertices(0), nIndices(0) { }

	/*! Vertex array object used to draw this model. */
	GLuint VAO;

	/* Vertex buffer object, containing Vertex structs. */
	GLuint VBO;

	/*! Vertex buffer object containing VertexBoneData structs. */
	GLuint VBO_bone;

	/*! Element buffer object. */
	GLuint EBO;

	/*! Total number of vertices in VBO. */
	GLuint nVertices;

	/*! Total number of indices in EBO. */
	GLuint nIndices;

	/*! Bone data of the mesh. */
	std::vector<BoneData> boneData;

	/*! Cached transforms returned from internal method. */
	std::vector<glm::mat4> boneTransforms;
};
