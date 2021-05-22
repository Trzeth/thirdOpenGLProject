#pragma once
#include "Model.h"
#include "Texture.h"

#include <glm/glm.hpp>

struct HDRSkyboxReturn {
	Model HDRSkybox;
	Texture Irradiancemap;
	Texture BRDFLUT;
	Texture Prefiltermap;
};

class Box
{
public:
	static Model GetBox(const std::vector<Texture>& textures, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 offset = glm::vec3(0.0f));
	static Model GetSkybox(const std::string& directory, const std::vector<std::string>& filename);

	// Desperated maybe no working
	static HDRSkyboxReturn GetHDRSkybox(const std::string& filename);
	static Model GetPlane(const std::vector<Texture>& textures, glm::vec3 ubasis, glm::vec3 vbasis, glm::vec2 dimensions, glm::vec2 textureOffset = glm::vec2(0.0f), glm::vec2 textureScale = glm::vec2(1.0f));
};
