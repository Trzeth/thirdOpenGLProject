#include "ShaderImpl.h"
#include "RenderUtil.h"

ShaderImpl::ShaderImpl()
	: shaderID(0), projectionUniform(0), viewUniform(0), modelUniform(0)
{ }

ShaderImpl::ShaderImpl(GLuint shaderID)
	: shaderID(shaderID)
{
	this->projectionUniform = this->GetUniformLocation("projection");
	this->viewUniform = this->GetUniformLocation("view");
	this->modelUniform = this->GetUniformLocation("model");
}

void ShaderImpl::Use() const
{
	glUseProgram(shaderID);
	glCheckError();
}

GLuint ShaderImpl::GetID() const
{
	return shaderID;
}

GLint ShaderImpl::GetUniformLocation(const std::string& uniformIdentifier) const
{
	return glGetUniformLocation(shaderID, uniformIdentifier.c_str());
}

void ShaderImpl::SetViewMatrix(const glm::mat4& matrix) const
{
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, &matrix[0][0]);
	glCheckError();
}

void ShaderImpl::SetProjectionMatrix(const glm::mat4& matrix) const
{
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &matrix[0][0]);
	glCheckError();
}

void ShaderImpl::SetModelMatrix(const glm::mat4& matrix) const
{
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, &matrix[0][0]);
	glCheckError();
}