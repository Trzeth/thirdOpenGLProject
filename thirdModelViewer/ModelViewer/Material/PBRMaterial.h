#pragma once
#include <thirdEngine/Renderer/Material.h>

class PBRMaterial :public Material
{
public:
	PBRMaterial() {
		properties = std::map<std::string, MaterialProperty>{
			{"albedo",MaterialProperty(glm::vec3(1.0,1.0,1.0))},
			{"metallic",MaterialProperty(0.4)},
			{"roughness",MaterialProperty(0.4)},
			{"ao",MaterialProperty(0.4)},
		};
	};
};