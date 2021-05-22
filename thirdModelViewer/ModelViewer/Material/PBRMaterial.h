#pragma once
#include <thirdEngine/Renderer/Material.h>

class PBRMaterial :public Material
{
public:
	PBRMaterial() {
		SetProperty("albedo", MaterialProperty(glm::vec3(1.0, 1.0, 1.0)));
		SetProperty("metallic", MaterialProperty(0.4f));
		SetProperty("roughness", MaterialProperty(0.4f));
		SetProperty("ao", MaterialProperty(0.4f));
		SetProperty("lightPositions[0]", MaterialProperty(glm::vec3(10.0, 0.0, 0.0)));
		SetProperty("lightColors[0]", MaterialProperty(glm::vec3(1.0, 1.0, 1.0)));
	};
};