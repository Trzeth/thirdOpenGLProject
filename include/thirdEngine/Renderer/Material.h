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
	MaterialPropertyValue(const Texture& texture) : texture(texture) { }
	MaterialPropertyValue(Texture&& texture) : texture(std::move(texture)) { }
	~MaterialPropertyValue() { }

	MaterialPropertyValue(const MaterialPropertyValue& other) = delete;
	MaterialPropertyValue& operator=(const MaterialPropertyValue& other) = delete;

	glm::vec3 vec3;
	glm::vec4 vec4;
	float flt;
	Texture texture;
};

struct MaterialProperty
{
	MaterialProperty();
	MaterialProperty(glm::vec3 vec3);
	MaterialProperty(glm::vec4 vec4);
	MaterialProperty(float flt);
	MaterialProperty(const Texture& texture);
	MaterialProperty(Texture&& texture);
	~MaterialProperty();

	MaterialProperty(const MaterialProperty& property);
	MaterialProperty& operator=(const MaterialProperty& other);

	MaterialProperty(MaterialProperty&& other) noexcept;
	MaterialProperty& operator=(MaterialProperty&& other) noexcept;

	MaterialPropertyType type;
	std::unique_ptr<MaterialPropertyValue> value;
	// By convention, the max of this field is 56 bytes
	uint64_t propertyId;
};

class Material
{
public:
	Material();
	~Material() = default;

	Material(const Material& other) = default;
	Material& operator=(const Material& other) = default;

	Material(Material&& other) noexcept;
	Material& operator=(Material&& other) noexcept;

	void SetProperty(const std::string& key, const MaterialProperty& property);
	void SetProperty(const std::string& key, MaterialProperty&& property);

	void SetTextures(const std::vector<Texture>& textures);
	void SetTextures(std::vector<Texture>&& textures);

	void Apply(const std::shared_ptr<Shader>& shader)const;
	void Apply(const ShaderImpl& shader)const;

	std::map<std::string, MaterialProperty>& GetProperties();

	MaterialDrawOrder DrawOrder;
	MaterialDrawType DrawType;
protected:
	std::map<std::string, MaterialProperty> properties;
};
