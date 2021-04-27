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
	~Shader();

	Shader(const Shader& shader) = delete;
	Shader& operator=(const Shader& shader) = delete;

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	bool IsValid();
	std::unique_ptr<ShaderImpl> impl;
};

class ShaderLoader {
public:
	Shader BuildFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	Shader BuildFromString(const std::string& vertexShader, const std::string& fragmentShader);
};