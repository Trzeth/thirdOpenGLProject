#include <glad/glad.h>

#include "Material.h"

static uint64_t nextId;
static std::unordered_map<uint64_t, GLint> shaderUniformCache;

MaterialProperty::MaterialProperty() :type(MaterialPropertyType::Invalid), propertyId(0)
{ }

MaterialProperty::MaterialProperty(const MaterialProperty& property)
	: type(property.type)
	, value(new MaterialPropertyValue(*property.value))
	, propertyId(property.propertyId)
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
{
}

MaterialProperty::~MaterialProperty()
{ }

Material::Material()
	:DrawOrder(MaterialDrawOrder::Less), DrawType(MaterialDrawType::Triangles)
{
}

void Material::SetProperty(const std::string& key, const MaterialProperty& property)
{
	auto iter = properties.find(key);

	if (iter != properties.end()) {
		iter->second.type = property.type;
		iter->second.value = std::make_unique<MaterialPropertyValue>(*property.value);
	}
	else {
		MaterialProperty copyProp;
		copyProp.propertyId = nextId++;
		copyProp.type = property.type;
		copyProp.value = std::make_unique<MaterialPropertyValue>(*property.value);
		properties.insert(std::make_pair(key, copyProp));
	}
}

void Material::SetTextures(const std::vector<Texture>& textures)
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

void Material::Apply(const Shader& shader) const
{
	this->Apply(*shader.impl);
}

void Material::Apply(const ShaderImpl& shader) const
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
			if (property.value->texture.type == TextureType::Cubemap) {
				glBindTexture(GL_TEXTURE_CUBE_MAP, property.value->texture.id);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, property.value->texture.id);
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