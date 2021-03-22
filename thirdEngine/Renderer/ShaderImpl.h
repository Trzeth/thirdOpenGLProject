#pragma once
#include <string>
#include <glm/glm.hpp>

#include <glad/glad.h>

class ShaderImpl
{
public:
	ShaderImpl();
	ShaderImpl(GLuint shaderID);

	void Use() const;
	GLuint GetID() const;

	GLint GetUniformLocation(const std::string& uniformIdentifier) const;
	void SetViewMatrix(const glm::mat4& matrix) const;
	void SetProjectionMatrix(const glm::mat4& matrix) const;
	void SetModelMatrix(const glm::mat4& matrix) const;
private:
	GLuint shaderID;
	GLint projectionUniform, viewUniform, modelUniform;
};
