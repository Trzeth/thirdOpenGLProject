#pragma once
#include <glad/glad.h>
#include "Texture.h"

struct TextureImpl
{
	TextureImpl() :type(TextureType::Diffuse), id(0) { };
	TextureImpl(TextureType type, GLuint id) : type(type), id(id) { };

	TextureType type;
	GLuint id;
};
