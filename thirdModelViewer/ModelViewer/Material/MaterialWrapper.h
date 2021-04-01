#pragma once
#include <thirdEngine/Renderer/Material.h>
struct ShaderFileInfo {
	ShaderFileInfo(std::string vertexShaderPath, std::string fragmentShaderPath)
		: vertexShaderPath(vertexShaderPath), fragmentShaderPath(fragmentShaderPath) { }
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
};

struct MaterialWrapper
{
	MaterialWrapper(std::string name, ShaderFileInfo fileInfo, Material material);

	std::string name;
	ShaderFileInfo shaderFileInfo;
	Shader shader;
	Material material;
};