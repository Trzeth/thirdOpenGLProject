#pragma once
#include <memory>
#include <vector>
#include <string>

enum class TextureType {
	Diffuse,
	Specular,
	Cubemap,
	HDR
};

struct TextureImpl;

struct Texture
{
	Texture();
	Texture(std::unique_ptr<TextureImpl>&& impl);

	~Texture();

	Texture(const Texture& other);
	Texture& operator=(const Texture& other);

	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	std::unique_ptr<TextureImpl> impl;
};

struct TextureLoader
{
	Texture LoadFromFile(TextureType type, const std::string& imageLocation);
	Texture LoadCubemap(const std::string& directory, const std::vector<std::string>& filename);
	Texture LoadHDR(const std::string& filename);
};