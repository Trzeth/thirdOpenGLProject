#include "Box.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

static const GLfloat vertex_data[] = {
	// Positions           // Normals           // Texture Coords
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
};

Model Box::GetBox(const std::vector<Texture>& textures, glm::vec3 scale, glm::vec3 offset)
{
	std::vector<Vertex> vertices(36);
	for (unsigned i = 0; i < vertices.size(); i++) {
		vertices[i].position = glm::vec3(vertex_data[i * 8] * scale.x, vertex_data[i * 8 + 1] * scale.y, vertex_data[i * 8 + 2] * scale.z) + offset;
		vertices[i].normal = glm::vec3(vertex_data[i * 8 + 3], vertex_data[i * 8 + 4], vertex_data[i * 8 + 5]);
		vertices[i].texCoords = glm::vec2(vertex_data[i * 8 + 6], vertex_data[i * 8 + 7]);
	}

	std::vector<GLuint> indexes(36);
	for (unsigned i = 0; i < indexes.size(); i++) {
		indexes[i] = i;
	}

	Material material;
	material.SetTextures(textures);

	Mesh mesh(vertices, indexes, material);
	Model model;
	model.meshes = std::vector<Mesh>{ mesh };
	model.meshesTransform = std::vector<std::vector<glm::mat4>>{ std::vector<glm::mat4>{glm::mat4(1.0)} };
	return model;
}

GLfloat skybox_verts[] = {
	// Positions
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

Model Box::GetSkybox(const std::string& directory, const std::vector<std::string>& filename)
{
	std::vector<Vertex> vertices(36);
	for (unsigned int i = 0; i < vertices.size(); i++) {
		vertices[i].position = glm::vec3(skybox_verts[i * 3], skybox_verts[i * 3 + 1], skybox_verts[i * 3 + 2]);
	}

	std::vector<GLuint> indexes(36);
	for (unsigned int i = 0; i < indexes.size(); i++) {
		indexes[i] = i;
	}

	std::vector<Texture> textures;
	TextureLoader textureLoader;
	Texture texture = textureLoader.LoadCubemap(directory, filename);
	textures.push_back(texture);

	Material material;
	material.SetTextures(textures);

	Mesh mesh(vertices, indexes, material);

	Model model;
	model.meshes = std::vector<Mesh>{ mesh };
	model.meshesTransform = std::vector<std::vector<glm::mat4>>{ std::vector<glm::mat4>{glm::mat4(1.0)} };
	return model;
}

Model Box::GetHDRSkybox(const std::string& filename)
{
	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	ShaderLoader shaderLoader;

	const std::string vs("#version 330 core\nlayout (location = 0) in vec3 aPos;out vec3 localPos;uniform mat4 projection;uniform mat4 view;void main(){localPos=aPos;gl_Position=projection*view*vec4(localPos,1.0);}");
	const std::string fs("#version 330 core\nout vec4 FragColor;in vec3 localPos;uniform sampler2D equirectangularMap;const vec2 invAtan = vec2(0.1591, 0.3183);vec2 SampleSphericalMap(vec3 v){vec2 uv = vec2(atan(v.z, v.x), asin(v.y));uv *= invAtan;uv += 0.5;return uv;}void main(){vec2 uv = SampleSphericalMap(normalize(localPos));vec3 color = texture(equirectangularMap, uv).rgb;FragColor = vec4(color, 1.0);}");
	Shader equirectangularToCubemapShader = shaderLoader.BuildFromString(vs, fs);

	// convert HDR equirectangular environment map to cubemap equivalent
	equirectangularToCubemapShader.impl->Use();
	glUniform1i(equirectangularToCubemapShader.impl->GetUniformLocation("equirectangularMap"), 0);
	equirectangularToCubemapShader.impl->SetProjectionMatrix(captureProjection);

	TextureLoader textureLoader;
	Texture hdrTexture = textureLoader.LoadHDR(filename);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture.impl->id);

	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	Model box = GetBox(std::vector<Texture>());
	box.GenVAO();
	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangularToCubemapShader.impl->SetViewMatrix(captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		box.meshes[0].Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Texture cubemap(std::make_unique<TextureImpl>(TextureType::Cubemap, envCubemap));

	std::vector<Vertex> vertices(36);
	for (unsigned int i = 0; i < vertices.size(); i++) {
		vertices[i].position = glm::vec3(skybox_verts[i * 3], skybox_verts[i * 3 + 1], skybox_verts[i * 3 + 2]);
	}

	std::vector<GLuint> indexes(36);
	for (unsigned int i = 0; i < indexes.size(); i++) {
		indexes[i] = i;
	}

	Material material;
	material.SetTextures(std::vector<Texture>{cubemap});

	Mesh mesh(vertices, indexes, material);

	Model model;
	model.meshes = std::vector<Mesh>{ mesh };
	model.meshesTransform = std::vector<std::vector<glm::mat4>>{ std::vector<glm::mat4>{glm::mat4(1.0)} };

	return model;
}

Model Box::GetPlane(const std::vector<Texture>& textures, glm::vec3 ubasis, glm::vec3 vbasis, glm::vec2 dimensions, glm::vec2 textureOffset, glm::vec2 textureScale)
{
	float ltc = textureOffset.x;
	float rtc = textureOffset.x + textureScale.x;
	float btc = textureOffset.y;
	float ttc = textureOffset.y + textureScale.y;

	std::vector<Vertex> vertices(4);
	vertices[0].position = (-ubasis * dimensions.x - vbasis * dimensions.y) * 0.5f;
	vertices[0].texCoords = glm::vec2(ltc, ttc);
	vertices[1].position = (ubasis * dimensions.x - vbasis * dimensions.y) * 0.5f;
	vertices[1].texCoords = glm::vec2(rtc, ttc);
	vertices[2].position = (-ubasis * dimensions.x + vbasis * dimensions.y) * 0.5f;
	vertices[2].texCoords = glm::vec2(ltc, btc);
	vertices[3].position = (ubasis * dimensions.x + vbasis * dimensions.y) * 0.5f;
	vertices[3].texCoords = glm::vec2(rtc, btc);
	vertices[0].normal = vertices[1].normal = vertices[2].normal = vertices[3].normal = glm::cross(ubasis, vbasis);

	std::vector<unsigned> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);

	Material material;
	material.SetTextures(textures);
	Mesh mesh(vertices, indices, material);

	Model model;
	model.meshes = std::vector<Mesh>{ mesh };
	return model;
}