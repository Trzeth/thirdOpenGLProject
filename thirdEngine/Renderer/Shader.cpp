#include "Shader.h"
#include "RenderUtil.h"

#include <glad/glad.h>

using namespace std;

Shader::Data::~Data()
{
	glDeleteProgram(id);
};

Shader::Shader()
	:data()
{
	projectionUniform = 0;
	viewUniform = 0;
	modelUniform = 0;
	viewPosUniform = 0;
}

Shader::Shader(std::shared_ptr<Shader::Data> data)
	: data(data)
{
	projectionUniform = this->GetUniformLocation("projection");
	viewUniform = this->GetUniformLocation("view");
	modelUniform = this->GetUniformLocation("model");
	viewPosUniform = this->GetUniformLocation("viewPos");
}

void Shader::Use() const
{
	glUseProgram(data->id);
	glCheckError();
}

GLuint Shader::GetID() const
{
	return data->id;
}

GLint Shader::GetUniformLocation(const std::string& uniformIdentifier) const
{
	return glGetUniformLocation(data->id, uniformIdentifier.c_str());
}

void Shader::SetViewMatrix(const glm::mat4& matrix) const
{
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, &matrix[0][0]);
	glCheckError();
}

void Shader::SetProjectionMatrix(const glm::mat4& matrix) const
{
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &matrix[0][0]);
	glCheckError();
}

void Shader::SetModelMatrix(const glm::mat4& matrix) const
{
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, &matrix[0][0]);
	glCheckError();
}

void Shader::SetViewPos(const glm::vec3& pos) const
{
	glUniform3f(viewPosUniform, pos.x, pos.y, pos.z);
	glCheckError();
}

bool Shader::IsValid()
{
	return data != nullptr;
}

Shader ShaderLoader::BuildFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	// 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	// ��֤ifstream��������׳��쳣��
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		// ���ļ�
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);
		stringstream vShaderStream, fShaderStream;
		// ��ȡ�ļ��Ļ������ݵ���������
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// �ر��ļ�������
		vShaderFile.close();
		fShaderFile.close();
		// ת����������string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n%s\n%s\n", vertexShaderPath.c_str(), fragmentShaderPath.c_str());
	}

	return BuildFromString(vertexCode, fragmentCode);
}

Shader ShaderLoader::BuildFromString(const std::string& vertexShader, const std::string& fragmentShader)
{
	// 2. ������ɫ��
	const char* vShaderCode = vertexShader.c_str();
	const char* fShaderCode = fragmentShader.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// ������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// ��ӡ�����������еĻ���
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
	};

	// Ƭ����ɫ��Ҳ����

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// ��ӡ�����������еĻ���
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
	};

	// ��ɫ������
	GLuint ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// ��ӡ���Ӵ�������еĻ���
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
	}

	// ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
	glDetachShader(ID, vertex);
	glDetachShader(ID, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	/*
#if _DEBUG
	GLint uniformCount;
	glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &uniformCount);
	printf("INFO::SHADER::ACTIVE_UNIFORM \n%s\n%s\n", vertexShaderPath.c_str(), fragmentShaderPath.c_str());
	char buffer[128];
	for (GLint i = 0; i < uniformCount; i++) {
		GLsizei length;
		glGetActiveUniformName(ID, i, 128, &length, buffer);
		printf("%s\n", buffer);
	}
#endif
*/

	return Shader(make_shared<Shader::Data>(ID));
}