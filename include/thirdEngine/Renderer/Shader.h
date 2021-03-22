#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

class ShaderImpl;

struct Shader
{
	Shader();
	Shader(GLuint shaderID);
	Shader(std::unique_ptr<ShaderImpl>&& impl);
	Shader(const Shader& shader);
	~Shader();

	void operator=(const Shader& shader);
	bool IsValid();
	std::unique_ptr<ShaderImpl> impl;
};

class ShaderLoader {
public:
	Shader CompileAndLink(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
};