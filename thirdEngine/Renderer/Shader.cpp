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
	// 1. 从文件路径中获取顶点/片段着色器
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		// 打开文件
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);
		stringstream vShaderStream, fShaderStream;
		// 读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		// 转换数据流到string
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
	// 2. 编译着色器
	const char* vShaderCode = vertexShader.c_str();
	const char* fShaderCode = fragmentShader.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// 顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// 打印编译错误（如果有的话）
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
	};

	// 片段着色器也类似

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// 打印编译错误（如果有的话）
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
	};

	// 着色器程序
	GLuint ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// 打印连接错误（如果有的话）
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
	}

	// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
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