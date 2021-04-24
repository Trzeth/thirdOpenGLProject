#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include <memory>

#include <glm/glm.hpp>

#include "Shader.h"
#include "ShaderImpl.h"
#include "Texture.h"
#include "TextureImpl.h"

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

union MaterialPropertyValue {
	MaterialPropertyValue() { memset(this, 0, sizeof(MaterialPropertyValue)); }
	MaterialPropertyValue(glm::vec3 vec3) : vec3(vec3) { }
	MaterialPropertyValue(glm::vec4 vec4) : vec4(vec4) { }
	MaterialPropertyValue(float flt) : flt(flt) { }
	MaterialPropertyValue(const Texture& texture) : texture(*texture.impl) { }
	~MaterialPropertyValue() { }

	glm::vec3 vec3;
	glm::vec4 vec4;
	float flt;
	TextureImpl texture;
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

	std::map<std::string, MaterialProperty>& GetProperties();

	MaterialDrawOrder DrawOrder;
	MaterialDrawType DrawType;
protected:
	std::map<std::string, MaterialProperty> properties;
};
