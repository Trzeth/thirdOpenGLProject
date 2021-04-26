#include <glad/glad.h>
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Texture.h"
#include "TextureImpl.h"
#include "RenderUtil.h"

Texture::Texture()
	: impl(new TextureImpl())
{ }

Texture::Texture(std::unique_ptr<TextureImpl>&& impl)
{
	this->impl = std::move(impl);
}

Texture::~Texture()
{
	if (!impl)
		return;

	glDeleteTextures(1, &impl->id);
}

Texture::Texture(const Texture& other)
{
	*this = other;
}

Texture& Texture::operator=(const Texture& other)
{
#ifdef _DEBUG
	printf("Texture Copy\n");
#endif // _DEBUG

	impl = std::make_unique<TextureImpl>();
	impl->type = other.impl->type;

	int w, h;
	int miplevel = 0;
	glBindTexture(GL_TEXTURE_2D, other.impl->id);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &impl->id);
	glBindTexture(GL_TEXTURE_2D, impl->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);
	/* Check the image interal format assert as RGB */
	glCheckError();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, other.impl->id, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, impl->id, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(0, 0, w, h, 0, 0, w, h,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glCheckError();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fbo);

	glCheckError();

	return *this;
}

Texture::Texture(Texture&& other) noexcept = default;

Texture& Texture::operator=(Texture && other) noexcept = default;

Texture TextureLoader::LoadFromFile(TextureType type, const std::string & imageLocation)
{
	Texture texture;
	texture.impl = std::make_unique<TextureImpl>();
	texture.impl->type = type;

	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glCheckError();

	int width, height, nrComponents;
	unsigned char* data = stbi_load(imageLocation.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glCheckError();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glCheckError();

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}
	else
	{
		printf("Texture failed to load at path: %s \n", imageLocation.c_str());
		stbi_image_free(data);
	}

	texture.impl->id = textureID;
	return texture;
}

Texture TextureLoader::LoadCubemap(const std::string & directory, const std::vector<std::string>&filename)
{
	Texture texture;
	texture.impl = std::make_unique<TextureImpl>();
	texture.impl->type = TextureType::Cubemap;

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glCheckError();

	assert(filename.size() == 6);

	std::string::const_iterator it = directory.end();
	it--;
	std::string baseDir = (*it == '/') ? directory : directory + "/";

	for (unsigned int i = 0; i != filename.size(); i++) {
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load((baseDir + filename[i]).c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glCheckError();

			stbi_image_free(data);
		}
		else
		{
			printf("Texture failed to load at path: %s \n", (baseDir + filename[i]).c_str());
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glCheckError();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	texture.impl->id = id;
	return texture;
}

Texture TextureLoader::LoadHDR(const std::string & filename)
{
	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glCheckError();

	Texture texture;
	texture.impl = std::make_unique<TextureImpl>();
	texture.impl->type = TextureType::HDR;
	texture.impl->id = textureID;

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glCheckError();

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}
	else
	{
		printf("Texture failed to load at path: %s \n", filename.c_str());
	}

	stbi_set_flip_vertically_on_load(false);
	return texture;
}