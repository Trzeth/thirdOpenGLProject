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
	MaterialWrapper(std::string name, ShaderFileInfo fileInfo, const std::shared_ptr<Material>& material);

	std::string name;
	ShaderFileInfo shaderFileInfo;
	Shader shader;
	std::shared_ptr<Material> material;
};