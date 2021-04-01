#include "MaterialWrapper.h"

#include <thirdEngine/Renderer/Shader.h>

static ShaderLoader shaderLoader;

MaterialWrapper::MaterialWrapper(std::string name, ShaderFileInfo fileInfo, Material material)
	:name(name), shaderFileInfo(fileInfo), material(material)
{
	shader = shaderLoader.CompileAndLink(fileInfo.vertexShaderPath, fileInfo.fragmentShaderPath);
}