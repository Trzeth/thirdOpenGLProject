#pragma once
#include <thirdEngine/Renderer/Material.h>

class MaterialLoader
{
public:
	/*!
	 * @brief Load material from material file(.thirdmtl)
	 * @param filepath
	 * @return
	*/
	Material LoadFromFile(const std::string& filepath);
};
