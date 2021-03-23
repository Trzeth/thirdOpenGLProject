#pragma once
#include <string>
#include <memory>

#include "Model.h"

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	Model LoadModel(const std::string& path);

	void SetDefaultMaterial(const Material& material);
private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};
