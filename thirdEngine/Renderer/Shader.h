#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>

typedef unsigned int GLuint;
typedef int GLint;

class Shader
{
public:
	struct Data;

	Shader();
	Shader(std::shared_ptr<Shader::Data> data);

	void Use() const;
	GLuint GetID() const;

	GLint GetUniformLocation(const std::string& uniformIdentifier) const;

	/*
	template<typename T>
	bool SetUniform(const std::string& variableName, T var);
	*/

	void SetViewMatrix(const glm::mat4& matrix) const;
	void SetProjectionMatrix(const glm::mat4& matrix) const;
	void SetModelMatrix(const glm::mat4& matrix) const;
	void SetViewPos(const glm::vec3& pos)const;

	bool IsValid();
private:
	std::shared_ptr<Data> data;
	GLint projectionUniform, viewUniform, modelUniform, viewPosUniform;
};

struct Shader::Data
{
	Data() :id(0) { }
	Data(GLuint id) :id(id) { }

	Data(const Data& other) = delete;
	Data& operator=(const Data& other) = delete;

	Data(Data&& d) noexcept {
		*this = std::move(d);
	}
	Data& operator=(Data&& other) noexcept {
		id = other.id;
		other.id = 0;
		return *this;
	}

	~Data();

	//Shader Id
	GLuint id;
};

class ShaderLoader {
public:
	Shader BuildFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	Shader BuildFromString(const std::string& vertexShader, const std::string& fragmentShader);
};