#pragma once
#include <string>
#include <memory>

#include "Model.h"

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	/*!
	 * @brief Load model from file
	 * @param path
	 * @param preDefineTransform Apply the transform matrix on the root node
	 * @return
	*/
	Model LoadModel(const std::string& path, glm::mat4 preDefineTransform = glm::mat4(1.0f));

	void SetDefaultMaterial(const std::shared_ptr<Material>& material);
private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};
