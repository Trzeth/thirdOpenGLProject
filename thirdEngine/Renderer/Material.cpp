#include <glad/glad.h>

#include "Material.h"

static uint64_t nextId;
static std::unordered_map<uint64_t, GLint> shaderUniformCache;

MaterialProperty::MaterialProperty() :type(MaterialPropertyType::Invalid), value(), propertyId(0)
{ }

MaterialProperty::MaterialProperty(glm::vec3 vec3)
	: type(MaterialPropertyType::Vec3)
	, value(new MaterialPropertyValue(vec3))
	, propertyId(0)
{ }

MaterialProperty::MaterialProperty(glm::vec4 vec4)
	: type(MaterialPropertyType::Vec4)
	, value(new MaterialPropertyValue(vec4))
	, propertyId(0)
{
}

MaterialProperty::MaterialProperty(float flt)
	: type(MaterialPropertyType::Float)
	, value(new MaterialPropertyValue(flt))
	, propertyId(0)
{
}

MaterialProperty::MaterialProperty(const Texture& texture)
	: type(MaterialPropertyType::Texture)
	, value(new MaterialPropertyValue(texture))
	, propertyId(0)
{ }

MaterialProperty::MaterialProperty(Texture&& texture)
	: type(MaterialPropertyType::Texture)
	, value(new MaterialPropertyValue(std::move(texture)))
	, propertyId(0)
{ }

MaterialProperty::~MaterialProperty()
{
	if (value && type == MaterialPropertyType::Texture)
		value->texture.~Texture();
}

MaterialProperty::MaterialProperty(const MaterialProperty& property)
{
	*this = property;
}

MaterialProperty& MaterialProperty::operator=(const MaterialProperty& other)
{
#ifdef _DEBUG
	printf("MaterialProperty Copy\n");
#endif // _DEBUG

	type = other.type;
	propertyId = 0;
	switch (type)
	{
	case MaterialPropertyType::Vec3:
		value = std::make_unique<MaterialPropertyValue>(other.value->vec3);
		break;
	case MaterialPropertyType::Vec4:
		value = std::make_unique<MaterialPropertyValue>(other.value->vec4);
		break;
	case MaterialPropertyType::Float:
		value = std::make_unique<MaterialPropertyValue>(other.value->flt);
		break;
	case MaterialPropertyType::Texture:
		value = std::make_unique<MaterialPropertyValue>(other.value->texture);
		break;
	case MaterialPropertyType::Invalid:
		break;
	default:
		break;
	}
	return *this;
}

MaterialProperty::MaterialProperty(MaterialProperty&& other) noexcept = default;

MaterialProperty& MaterialProperty::operator=(MaterialProperty && other) noexcept = default;

Material::Material()
	:DrawOrder(MaterialDrawOrder::Less), DrawType(MaterialDrawType::Triangles)
{
}

Material::Material(Material && other) noexcept
{
	*this = std::move(other);
}

Material& Material::operator=(Material && other) noexcept
{
	properties.merge(other.properties);

	return *this;
}

void Material::SetProperty(const std::string & key, const MaterialProperty & property)
{
	auto iter = properties.find(key);

	if (iter != properties.end()) {
		iter->second = MaterialProperty(property);
	}
	else {
		MaterialProperty copyProp(property);
		copyProp.propertyId = nextId++;

		properties.insert(std::make_pair(key, copyProp));
	}
}

void Material::SetProperty(const std::string & key, MaterialProperty && property)
{
	auto iter = properties.find(key);

	if (iter != properties.end()) {
		iter->second = std::move(property);
	}
	else {
		property.propertyId = nextId++;
		properties.insert(std::make_pair(key, std::move(property)));
	}
}

void Material::SetTextures(const std::vector<Texture>&textures)
{
	for (unsigned int i = 0; i < textures.size(); i++) {
		MaterialProperty textureProperty(textures[i]);
		std::string key;
		if (textures[i].impl->type == TextureType::Diffuse) {
			key = "texture_diffuse";
		}
		else if (textures[i].impl->type == TextureType::Specular) {
			key = "texture_specular";
		}
		else if (textures[i].impl->type == TextureType::Cubemap) {
			key = "cubemap";
			// not all cubemaps are skyboxes, but they are for now!
			this->DrawOrder = MaterialDrawOrder::LEqual;
		}
		this->SetProperty(key, textureProperty);
	}
}

void Material::SetTextures(std::vector<Texture> && textures)
{
	for (unsigned int i = 0; i < textures.size(); i++) {
		MaterialProperty textureProperty = std::move(textures[i]);
		const TextureType& type = textureProperty.value->texture.impl->type;
		std::string key;

		if (type == TextureType::Diffuse) {
			key = "texture_diffuse";
		}
		else if (type == TextureType::Specular) {
			key = "texture_specular";
		}
		else if (type == TextureType::Cubemap) {
			key = "cubemap";
			// not all cubemaps are skyboxes, but they are for now!
			this->DrawOrder = MaterialDrawOrder::LEqual;
		}
		this->SetProperty(key, std::move(textureProperty));
	}
}

void Material::Apply(const std::shared_ptr<Shader>&shader) const
{
	this->Apply(*shader->impl);
}

void Material::Apply(const ShaderImpl & shader) const
{
	unsigned int curTexture = 0;

	for (auto iter = properties.begin(); iter != properties.end(); ++iter) {
		const std::string& propertyName = iter->first;
		const MaterialProperty& property = iter->second;

		// Simple mapping - assume we'll never have more than 2^56 total material properties and 2^8
		// shaders and that shaders are assigned ids sequentially (foolish to assume on all implementations?)
		assert(property.propertyId < ((uint64_t)1 << 56) && shader.GetID() < (1 << 8));

		uint64_t hash = property.propertyId << 8 | shader.GetID();
		auto cacheIter = shaderUniformCache.find(hash);

		if (cacheIter == shaderUniformCache.end()) {
			// Miss - get the shader uniform and store it
			GLint shaderUniformId = shader.GetUniformLocation(propertyName);
			auto insertIterPair = shaderUniformCache.insert(std::make_pair(hash, shaderUniformId));
			cacheIter = insertIterPair.first;
		}
		unsigned shaderUniformId = cacheIter->second;

		switch (property.type) {
		case MaterialPropertyType::Vec3:
			glUniform3f(shaderUniformId, property.value->vec3.x, property.value->vec3.y, property.value->vec3.z);
			break;
		case MaterialPropertyType::Vec4:
			glUniform4f(shaderUniformId, property.value->vec4.x, property.value->vec4.y, property.value->vec4.z, property.value->vec4.w);
			break;
		case MaterialPropertyType::Texture:
			glActiveTexture(GL_TEXTURE0 + curTexture); // Activate proper texture unit before binding
			if (property.value->texture.impl->type == TextureType::Cubemap) {
				glBindTexture(GL_TEXTURE_CUBE_MAP, property.value->texture.impl->id);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, property.value->texture.impl->id);
			}
			glUniform1i(shaderUniformId, curTexture);
			curTexture++;
			break;
		case MaterialPropertyType::Float:
			glUniform1f(shaderUniformId, property.value->flt);
			break;
		}
	}

	int depthFunc = GL_LESS;

	if (DrawOrder == MaterialDrawOrder::Less) {
		depthFunc = GL_LESS;
	}
	else if (DrawOrder == MaterialDrawOrder::LEqual) {
		depthFunc = GL_LEQUAL;
	}
	glDepthFunc(depthFunc);
}

std::map<std::string, MaterialProperty>& Material::GetProperties()
{
	return properties;
}