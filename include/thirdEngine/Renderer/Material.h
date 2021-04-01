#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include <memory>

#include <glm/glm.hpp>

#include "Shader.h"
#include "ShaderImpl.h"
#include "Texture.h"

enum class MaterialDrawOrder
{
	Less,
	LEqual
};

enum class MaterialDrawType
{
	Triangles,
	Lines
};

enum class MaterialPropertyType
{
	Vec3,
	Vec4,
	Texture,
	Float,
	Invalid
};

struct MaterialProperty
{
	MaterialProperty();
	MaterialProperty(const MaterialProperty& property);
	MaterialProperty(glm::vec3 vec3);
	MaterialProperty(glm::vec4 vec4);
	MaterialProperty(float flt);
	MaterialProperty(const Texture& texture);
	~MaterialProperty();

	union MaterialPropertyValue;
	MaterialPropertyType type;
	std::unique_ptr<MaterialPropertyValue> value;
	// By convention, the max of this field is 56 bytes
	uint64_t propertyId;
};

class Material
{
public:
	Material();

	void SetProperty(const std::string& key, const MaterialProperty& property);
	void SetTextures(const std::vector<Texture>& textures);
	void Apply(const Shader& shader)const;
	void Apply(const ShaderImpl& shader)const;

	std::map<std::string, MaterialProperty> GetProperties()const;

	MaterialDrawOrder DrawOrder;
	MaterialDrawType DrawType;
protected:
	std::map<std::string, MaterialProperty> properties;
};
