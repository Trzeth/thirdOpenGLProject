#include "Shader.h"
#include "ShaderImpl.h"

using namespace std;

Shader::Shader() : impl(new ShaderImpl())
{ }

Shader::Shader(GLuint shaderID)
	: impl(make_unique<ShaderImpl>(shaderID))
{ }

Shader::Shader(std::unique_ptr<ShaderImpl>&& impl)
	: impl(std::move(impl))
{ }

Shader::Shader(const Shader& shader)
	: impl(make_unique<ShaderImpl>(*shader.impl))
{ }

Shader::~Shader()
{ }

void Shader::operator=(const Shader& shader)
{
	this->impl = std::make_unique<ShaderImpl>(*shader.impl);
}

bool Shader::IsValid()
{
	return impl->GetID() != 0;
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

	return Shader(make_unique<ShaderImpl>(ID));
}