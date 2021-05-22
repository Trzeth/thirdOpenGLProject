#pragma once
#pragma once
#include <thirdEngine/Renderer/Material.h>

class TexturePBRMaterial :public Material
{
public:
	TexturePBRMaterial() {
		SetProperty("albedoMap", MaterialProperty(Texture()));
		SetProperty("normalMap", MaterialProperty(Texture()));
		SetProperty("metallicMap", MaterialProperty(Texture()));
		SetProperty("roughnessMap", MaterialProperty(Texture()));
		SetProperty("aoMap", MaterialProperty(Texture()));
		SetProperty("lightPositions[0]", MaterialProperty(glm::vec3(10.0, 0.0, 0.0)));
		SetProperty("lightColors[0]", MaterialProperty(glm::vec3(1.0, 1.0, 1.0)));
	};
};