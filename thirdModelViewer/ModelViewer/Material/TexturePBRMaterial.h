#pragma once
#pragma once
#include <thirdEngine/Renderer/Material.h>

class TexturePBRMaterial :public Material
{
public:
	TexturePBRMaterial() {
		SetProperty("albedoMap", MaterialProperty(Texture(std::make_unique<TextureImpl>())));
		SetProperty("normalMap", MaterialProperty(Texture(std::make_unique<TextureImpl>())));
		SetProperty("metallicMap", MaterialProperty(Texture(std::make_unique<TextureImpl>())));
		SetProperty("roughnessMap", MaterialProperty(Texture(std::make_unique<TextureImpl>())));
		SetProperty("aoMap", MaterialProperty(Texture(std::make_unique<TextureImpl>())));
		SetProperty("lightPositions[0]", MaterialProperty(glm::vec3(10.0, 0.0, 0.0)));
		SetProperty("lightColors[0]", MaterialProperty(glm::vec3(1.0, 1.0, 1.0)));
	};
};