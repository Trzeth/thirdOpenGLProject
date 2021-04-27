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

	Mesh mesh(vertices, indexes, std::move(material));
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

	TextureLoader textureLoader;
	std::vector<Texture> textures;
	textures.emplace_back(textureLoader.LoadCubemap(directory, filename));

	Material material;
	material.SetTextures(std::move(textures));

	Model model;
	model.meshes.emplace_back(Mesh(vertices, indexes, std::move(material)));
	model.meshesTransform = std::vector<std::vector<glm::mat4>>{ std::vector<glm::mat4>{glm::mat4(1.0)} };
	return model;
}

HDRSkyboxReturn Box::GetHDRSkybox(const std::string& filename)
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

	const std::string equirectangularToCubemapShaderVS("#version 330 core\nlayout (location = 0) in vec3 aPos;out vec3 localPos;uniform mat4 projection;uniform mat4 view;void main(){localPos=aPos;gl_Position=projection*view*vec4(localPos,1.0);}");
	const std::string equirectangularToCubemapShaderFS("#version 330 core\nout vec4 FragColor;in vec3 localPos;uniform sampler2D equirectangularMap;const vec2 invAtan = vec2(0.1591, 0.3183);vec2 SampleSphericalMap(vec3 v){vec2 uv = vec2(atan(v.z, v.x), asin(v.y));uv *= invAtan;uv += 0.5;return uv;}void main(){vec2 uv = SampleSphericalMap(normalize(localPos));vec3 color = texture(equirectangularMap, uv).rgb;FragColor = vec4(color, 1.0);}");
	Shader equirectangularToCubemapShader = shaderLoader.BuildFromString(equirectangularToCubemapShaderVS, equirectangularToCubemapShaderFS);

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

	// irradianceMap
	// -----------------------------------------------------------------------------

	unsigned int irradianceMap;
	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
	// -----------------------------------------------------------------------------
	const std::string irradianceShaderVS("#version 330 core\n layout (location = 0) in vec3 aPos; out vec3 WorldPos; uniform mat4 projection; uniform mat4 view; void main() { WorldPos = aPos;  gl_Position =  projection * view * vec4(WorldPos, 1.0); }");
	const std::string irradianceShaderFS("#version 330 core\n out vec4 FragColor; in vec3 WorldPos; uniform samplerCube environmentMap; const float PI = 3.14159265359; void main() {		vec3 N = normalize(WorldPos); vec3 irradiance = vec3(0.0);   vec3 up    = vec3(0.0, 1.0, 0.0); vec3 right = normalize(cross(up, N)); up         = normalize(cross(N, right)); float sampleDelta = 0.025; float nrSamples = 0.0; for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) { for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) { vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta)); vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta); nrSamples++; } } irradiance = PI * irradiance * (1.0 / float(nrSamples)); FragColor = vec4(irradiance, 1.0); }");
	Shader irradianceShader = shaderLoader.BuildFromString(irradianceShaderVS, irradianceShaderFS);

	irradianceShader.impl->Use();
	glUniform1i(irradianceShader.impl->GetUniformLocation("environmentMap"), 0);

	irradianceShader.impl->SetProjectionMatrix(captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		irradianceShader.impl->SetViewMatrix(captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		box.meshes[0].Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
// --------------------------------------------------------------------------------
	unsigned int prefilterMap;
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
	// ----------------------------------------------------------------------------------------------------
	const std::string prefilterShaderVS("#version 330 core\n layout (location = 0) in vec3 aPos; out vec3 WorldPos; uniform mat4 projection; uniform mat4 view; void main() { WorldPos = aPos;  gl_Position =  projection * view * vec4(WorldPos, 1.0); }");
	const std::string prefilterShaderFS("#version 330 core\n out vec4 FragColor; in vec3 WorldPos; uniform samplerCube environmentMap; uniform float roughness; const float PI = 3.14159265359; float DistributionGGX(vec3 N, vec3 H, float roughness) { float a = roughness*roughness; float a2 = a*a; float NdotH = max(dot(N, H), 0.0); float NdotH2 = NdotH*NdotH; float nom   = a2; float denom = (NdotH2 * (a2 - 1.0) + 1.0); denom = PI * denom * denom; return nom / denom; } float RadicalInverse_VdC(uint bits) { bits = (bits << 16u) | (bits >> 16u); bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u); bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u); bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u); bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u); return float(bits) * 2.3283064365386963e-10; } vec2 Hammersley(uint i, uint N) { return vec2(float(i)/float(N), RadicalInverse_VdC(i)); } vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness) { float a = roughness*roughness; float phi = 2.0 * PI * Xi.x; float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y)); float sinTheta = sqrt(1.0 - cosTheta*cosTheta); vec3 H; H.x = cos(phi) * sinTheta; H.y = sin(phi) * sinTheta; H.z = cosTheta; vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0); vec3 tangent   = normalize(cross(up, N)); vec3 bitangent = cross(N, tangent); vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z; return normalize(sampleVec); } void main() {		vec3 N = normalize(WorldPos); vec3 R = N; vec3 V = R; const uint SAMPLE_COUNT = 1024u; vec3 prefilteredColor = vec3(0.0); float totalWeight = 0.0; for(uint i = 0u; i < SAMPLE_COUNT; ++i) { vec2 Xi = Hammersley(i, SAMPLE_COUNT); vec3 H = ImportanceSampleGGX(Xi, N, roughness); vec3 L  = normalize(2.0 * dot(V, H) * H - V); float NdotL = max(dot(N, L), 0.0); if(NdotL > 0.0) { float D   = DistributionGGX(N, H, roughness); float NdotH = max(dot(N, H), 0.0); float HdotV = max(dot(H, V), 0.0); float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; float resolution = 512.0; float saTexel  = 4.0 * PI / (6.0 * resolution * resolution); float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001); float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL; totalWeight      += NdotL; } } prefilteredColor = prefilteredColor / totalWeight; FragColor = vec4(prefilteredColor, 1.0); }");
	Shader prefilterShader = shaderLoader.BuildFromString(prefilterShaderVS, prefilterShaderFS);

	prefilterShader.impl->Use();
	glUniform1i(prefilterShader.impl->GetUniformLocation("environmentMap"), 0);
	prefilterShader.impl->SetProjectionMatrix(captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		glUniform1f(prefilterShader.impl->GetUniformLocation("roughness"), roughness);

		for (unsigned int i = 0; i < 6; ++i)
		{
			prefilterShader.impl->SetViewMatrix(captureViews[i]);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			box.meshes[0].Draw();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// pbr: generate a 2D LUT from the BRDF equations used.
	// ----------------------------------------------------
	unsigned int brdfLUTTexture;
	glGenTextures(1, &brdfLUTTexture);

	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);

	const std::string brdfShaderVS("#version 330 core\n layout (location = 0) in vec3 aPos; layout (location = 1) in vec2 aTexCoords; out vec2 TexCoords; void main() { TexCoords = aTexCoords; gl_Position = vec4(aPos, 1.0); }");
	const std::string brdfShaderFS("#version 330 core\n out vec2 FragColor; in vec2 TexCoords; const float PI = 3.14159265359; float RadicalInverse_VdC(uint bits) { bits = (bits << 16u) | (bits >> 16u); bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u); bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u); bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u); bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u); return float(bits) * 2.3283064365386963e-10; } vec2 Hammersley(uint i, uint N) { return vec2(float(i)/float(N), RadicalInverse_VdC(i)); } vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness) { float a = roughness*roughness; float phi = 2.0 * PI * Xi.x; float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y)); float sinTheta = sqrt(1.0 - cosTheta*cosTheta); vec3 H; H.x = cos(phi) * sinTheta; H.y = sin(phi) * sinTheta; H.z = cosTheta; vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0); vec3 tangent   = normalize(cross(up, N)); vec3 bitangent = cross(N, tangent); vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z; return normalize(sampleVec); } float GeometrySchlickGGX(float NdotV, float roughness) { float a = roughness; float k = (a * a) / 2.0; float nom   = NdotV; float denom = NdotV * (1.0 - k) + k; return nom / denom; } float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) { float NdotV = max(dot(N, V), 0.0); float NdotL = max(dot(N, L), 0.0); float ggx2 = GeometrySchlickGGX(NdotV, roughness); float ggx1 = GeometrySchlickGGX(NdotL, roughness); return ggx1 * ggx2; } vec2 IntegrateBRDF(float NdotV, float roughness) { vec3 V; V.x = sqrt(1.0 - NdotV*NdotV); V.y = 0.0; V.z = NdotV; float A = 0.0; float B = 0.0; vec3 N = vec3(0.0, 0.0, 1.0); const uint SAMPLE_COUNT = 1024u; for(uint i = 0u; i < SAMPLE_COUNT; ++i) { vec2 Xi = Hammersley(i, SAMPLE_COUNT); vec3 H = ImportanceSampleGGX(Xi, N, roughness); vec3 L = normalize(2.0 * dot(V, H) * H - V); float NdotL = max(L.z, 0.0); float NdotH = max(H.z, 0.0); float VdotH = max(dot(V, H), 0.0); if(NdotL > 0.0) { float G = GeometrySmith(N, V, L, roughness); float G_Vis = (G * VdotH) / (NdotH * NdotV); float Fc = pow(1.0 - VdotH, 5.0); A += (1.0 - Fc) * G_Vis; B += Fc * G_Vis; } } A /= float(SAMPLE_COUNT); B /= float(SAMPLE_COUNT); return vec2(A, B); } void main() { vec2 integratedBRDF = IntegrateBRDF(TexCoords.x, TexCoords.y); FragColor = integratedBRDF; }");
	Shader brdfShader = shaderLoader.BuildFromString(brdfShaderVS, brdfShaderFS);
	brdfShader.impl->Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	unsigned int quadVAO;
	unsigned int quadVBO;

	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// setup plane VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// -----------------------------------------------------------------------------

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

	Mesh mesh(vertices, indexes, std::move(material));

	Model model;
	model.meshes = std::vector<Mesh>{ mesh };
	model.meshesTransform = std::vector<std::vector<glm::mat4>>{ std::vector<glm::mat4>{glm::mat4(1.0)} };

	HDRSkyboxReturn val;
	val.HDRSkybox = model;

	val.Irradiancemap = Texture(std::make_unique<TextureImpl>(TextureType::Cubemap, irradianceMap));
	val.Prefiltermap = Texture(std::make_unique<TextureImpl>(TextureType::Cubemap, prefilterMap));
	val.BRDFLUT = Texture(std::make_unique<TextureImpl>(TextureType::Diffuse, brdfLUTTexture));

	return val;
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
	Mesh mesh(vertices, indices, std::move(material));

	Model model;
	model.meshes = std::vector<Mesh>{ mesh };
	return model;
}