#include "MaterialWrapper.h"

#include <thirdEngine/Renderer/Shader.h>

static ShaderLoader shaderLoader;

MaterialWrapper::MaterialWrapper(std::string name, ShaderFileInfo fileInfo, const std::shared_ptr<Material>& material)
	:name(name), shaderFileInfo(fileInfo), material(material)
{
	shader = shaderLoader.BuildFromFile(fileInfo.vertexShaderPath, fileInfo.fragmentShaderPath);
}