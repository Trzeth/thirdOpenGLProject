#include "Shader.h"
#include "Renderer.h"
#include "RenderUtil.h"

#include "Mesh.h"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer()
{
	this->uiModelTransform = glm::mat4(1.0f);
	// Flip the y axis so we can use normal modelspace but position in UI space
	this->uiModelTransform[1][1] = -1.0f;
}

Renderer::ShaderCache::ShaderCache(const Shader& shader)
	:shader(shader), pointLights(maxPointLights), bones(maxBones)
{
	// Bind to bindpoint 0
	glUniformBlockBinding(shader.GetID(), glGetUniformBlockIndex(shader.GetID(), "baseMatrices"), 0);

	for (unsigned int i = 0; i < pointLights.size(); i++) {
		PointLightCache& light = this->pointLights[i];
		std::stringstream sstream;
		sstream << "pointLight[" << i << "]";
		light.constant = shader.GetUniformLocation((sstream.str() + ".constant").c_str());
		light.linear = shader.GetUniformLocation((sstream.str() + ".linear").c_str());
		light.quadratic = shader.GetUniformLocation((sstream.str() + ".quadratic").c_str());
		light.ambient = shader.GetUniformLocation((sstream.str() + ".ambient").c_str());
		light.diffuse = shader.GetUniformLocation((sstream.str() + ".diffuse").c_str());
		light.specular = shader.GetUniformLocation((sstream.str() + ".specular").c_str());
		light.position = shader.GetUniformLocation((sstream.str() + ".position").c_str());
		glCheckError();
	}

	this->dirLight.direction = shader.GetUniformLocation("dirLight.direction");
	this->dirLight.ambient = shader.GetUniformLocation("dirLight.ambient");
	this->dirLight.diffuse = shader.GetUniformLocation("dirLight.diffuse");
	this->dirLight.specular = shader.GetUniformLocation("dirLight.specular");

	this->pointLightCount = shader.GetUniformLocation("pointLightCount");

	for (unsigned int i = 0; i < maxBones; i++) {
		std::stringstream sstream;
		sstream << "bones[" << i << "]";
		this->bones[i] = shader.GetUniformLocation(sstream.str());
	}
}

void Renderer::Initialize(int width, int height)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		assert(0);
		//Failed to initialize GLAD
	}

	viewportWidth = width;
	viewportHeight = height;

	glGenBuffers(1, &baseMatrixUBO);

	glBindBuffer(GL_UNIFORM_BUFFER, baseMatrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, baseMatrixUBO, 0, 2 * sizeof(glm::mat4));
}

void Renderer::SetDirLight(const DirLight& dirLight)
{
	this->dirLight = dirLight;
}

void Renderer::SetProjectionMatrix(const glm::mat4& transform)
{
	this->projectionMatrix = transform;
}

void Renderer::SetViewMatrix(const glm::mat4& transform)
{
	this->viewMatrix = transform;
}

void Renderer::SetViewPos(const glm::vec3& pos)
{
	viewPos = pos;
}

void Renderer::SetRenderableAnimation(const RenderableHandle& handle, const std::string& animName, float time, bool autoPlay, bool loop)
{
	std::optional<std::reference_wrapper<Entity>> renderableOpt = entityPool.Get(handle);
	if (!renderableOpt) {
		return;
	}

	Entity& renderable = *renderableOpt;
	renderable.animName = animName;
	renderable.time = time;
	renderable.loopAnimation = loop;
	renderable.autoUpdate = autoPlay;
}

void Renderer::SetRenderableAnimationTime(const RenderableHandle& handle, float time)
{
	std::optional<std::reference_wrapper<Entity>> renderableOpt = entityPool.Get(handle);
	if (!renderableOpt) {
		return;
	}

	Entity& renderable = *renderableOpt;
	renderable.time = time;
}

void Renderer::SetRenderableModelHandle(const RenderableHandle& handle, const ModelHandle& modelHandle)
{
	std::optional<std::reference_wrapper<Entity>> renderableOpt = entityPool.Get(handle);
	if (!renderableOpt) {
		return;
	}

	Entity& renderable = *renderableOpt;
	renderable.modelHandle = modelHandle;
}

void Renderer::SetRenderableRenderSpace(const RenderableHandle& handle, RenderSpace space)
{
	std::optional<std::reference_wrapper<Entity>> renderableOpt = entityPool.Get(handle);
	if (!renderableOpt) {
		return;
	}

	Entity& renderable = *renderableOpt;
	renderable.space = space;
}

void Renderer::SetRenderableTransform(const RenderableHandle& handle, const glm::mat4& transform)
{
	std::optional<std::reference_wrapper<Entity>> renderableOpt = entityPool.Get(handle);
	if (!renderableOpt) {
		return;
	}

	Entity& renderable = *renderableOpt;
	renderable.transform = transform;
}

void Renderer::SetRenderableMaterial(const RenderableHandle& handle, const Material& material)
{
	std::optional<std::reference_wrapper<Entity>> renderableOpt = entityPool.Get(handle);
	if (!renderableOpt) {
		return;
	}

	modelPool.Get(renderableOpt->get().modelHandle)->get().material = material;
}

void Renderer::SetRenderableShader(const RenderableHandle& handle, const Shader& shader)
{
	auto shaderIter = shaderMap.find(shader.GetID());
	if (shaderIter == shaderMap.end()) {
		auto iterPair = shaderMap.emplace(std::make_pair(shader.GetID(), ShaderCache(shader)));
	}

	std::optional<std::reference_wrapper<Entity>> renderableOpt = entityPool.Get(handle);
	if (!renderableOpt) {
		return;
	}

	Entity& renderable = *renderableOpt;
	renderable.shaderCache.shader = shader;
}

void Renderer::SetViewport(int w, int h)
{
	SetViewport(0, 0, w, h);
}

void Renderer::Clear()
{
}

void Renderer::SetViewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);

	viewportWidth = w;
	viewportHeight = h;
}

void Renderer::GenVAO()
{
	for (auto iter = modelPool.begin(); iter != modelPool.end(); iter++) {
		Model& model = iter->second;
		model.GenVAO();
	}
}

int Renderer::GetViewportWidth() const
{
	return viewportWidth;
}

int Renderer::GetViewportHeight() const
{
	return viewportHeight;
}

float Renderer::GetRenderableAnimationTime(const RenderableHandle& handle)
{
	std::optional<std::reference_wrapper<Entity>> renderableOpt = entityPool.Get(handle);
	if (!renderableOpt) {
		return 0.0;
	}

	return renderableOpt->get().time;
}

Renderer::ModelHandle Renderer::GetModelHandle(Model model)
{
	return modelPool.GetNewHandle(model);
}

Renderer::RenderableHandle Renderer::GetRenderableHandle(const ModelHandle& modelHandle, const Shader& shader)
{
	auto shaderIter = shaderMap.find(shader.GetID());
	if (shaderIter == shaderMap.end()) {
		auto iterPair = shaderMap.emplace(std::make_pair(shader.GetID(), ShaderCache(shader)));
	}

	std::optional<std::reference_wrapper<Model>> modelOpt = modelPool.Get(modelHandle);
	if (!modelOpt) {
		return entityPool.InvalidHandle;
	}

	Model& model = *modelOpt;

	bool animatable = (model.data->animationData.animations.size() > 0);

	RenderableHandle handle = this->entityPool.GetNewHandle(Entity(shader, modelHandle, animatable));

	return handle;
}

void Renderer::ClearBuffer() const
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw()
{
	ClearBuffer();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	drawInternal(RenderSpace::World);
}

void Renderer::Update(float dt)
{
	for (auto iter = entityPool.begin(); iter != entityPool.end(); iter++) {
		Entity& renderable = iter->second;
		std::string animName = renderable.animName;
		if (!renderable.autoUpdate || animName.size() == 0) {
			continue;
		}

		renderable.time += dt;

		std::optional<std::reference_wrapper<Model>> modelOpt = modelPool.Get(iter->second.modelHandle);
		assert(modelOpt);

		// Deal with Later

		Model& model = *modelOpt;
		auto& animationMap = model.data->animationData.animations;
		auto animIter = animationMap.find(animName);

		if (animIter == animationMap.end()) {
			continue;
		}
		Animation& animation = animIter->second;

		float duration = animation.endTime - animation.startTime;
		if (renderable.time > duration) {
			// Loop or clamp
			if (renderable.loopAnimation) {
				renderable.time -= duration;
			}
			else {
				renderable.time = duration;
			}
		}
	}
}

void Renderer::drawInternal(RenderSpace space)
{
	glBindBuffer(GL_UNIFORM_BUFFER, baseMatrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewMatrix));

	for (auto iter = shaderMap.begin(); iter != shaderMap.end(); iter++) {
		const ShaderCache& shaderCache = iter->second;

		shaderCache.shader.Use();

		//??? Viewpos lightSpaceMartix skyBox Shader如何处理

		//Later

		//处理点光源 再说
		/*
		glUniform3f(shaderCache.dirLight.direction, dirLight.direction.x, dirLight.direction.y, dirLight.direction.z);
		glUniform3f(shaderCache.dirLight.ambient, dirLight.ambient.x, dirLight.ambient.y, dirLight.ambient.z);
		glUniform3f(shaderCache.dirLight.diffuse, dirLight.diffuse.x, dirLight.diffuse.y, dirLight.diffuse.z);
		glUniform3f(shaderCache.dirLight.specular, dirLight.specular.x, dirLight.specular.y, dirLight.specular.z);
		*/
		glCheckError();
	}

	// Calculate Frustum
	{
		frustum.resize(6);

		glm::mat4 matrix = glm::transpose(projectionMatrix * viewMatrix);
		frustum[0] = matrix[3] + matrix[0];
		frustum[1] = matrix[3] - matrix[0];
		frustum[2] = matrix[3] + matrix[1];
		frustum[3] = matrix[3] - matrix[1];
		frustum[4] = matrix[3] + matrix[2];
		frustum[5] = matrix[3] - matrix[2];

		for (int i = 0; i != frustum.size(); i++)
			frustum[i] /= glm::length2(glm::vec3(frustum[i]));
	}

	for (auto iter = entityPool.begin(); iter != entityPool.end(); iter++) {
		Entity& renderable = iter->second;
		if (renderable.space != space) {
			continue;
		}

		std::optional<std::reference_wrapper<Model>> modelOpt = modelPool.Get(renderable.modelHandle);
		assert(modelOpt);

		Model& model = *modelOpt;
		ShaderCache& shaderCache = renderable.shaderCache;
		glm::mat4 modelMatrix = renderable.transform;

		shaderCache.shader.Use();
		glCheckError();

		//只考虑了骨骼动画！！

		unsigned int cullMeshCount = 0;
		unsigned int totalMeshCount = 0;
		unsigned int vertexCount = 0;

		for (int i = 0; i != model.data->meshes.size(); i++)
		{
			const auto& mesh = model.data->meshes[i];

			//Do Anim once per mesh
			if (renderable.animatable) {
				std::vector<glm::mat4> nodeTransforms = model.GetNodeTransforms(renderable.animName, renderable.time, renderable.context);

				if (!mesh.hasVertexBoneData) {
					// Not skinned animation
				}
				else
				{
					//Skinned animation
					std::vector<glm::mat4> boneTransforms = mesh.GetBoneTransforms(nodeTransforms);
					for (unsigned int j = 0; j < boneTransforms.size(); j++) {
						glUniformMatrix4fv(shaderCache.bones[j], 1, GL_FALSE, &boneTransforms[j][0][0]);
					}
				}
			}

			// We assume mesh won't under animted node

			std::vector<glm::mat4> visibleMeshTransform;
			const auto& [buffer, transforms] = model.data->meshesTransform[i];
			for (const auto& transform : transforms) {
				totalMeshCount++;

				if (mesh.hasBoundingSphere)
				{
					/* Culling Per Mesh */

					bool visible = true;
					auto trans = modelMatrix * transform * glm::vec4(mesh.boundingSphere.center, 1);
					auto matrix = modelMatrix * transform;
					float maxScale = glm::max(glm::max(matrix[0][0], matrix[1][1]), matrix[2][2]);

					assert(frustum.size() == 6);
					for (int i = 0; i != frustum.size(); i++)
					{
						if (frustum[i].x * trans.x + frustum[i].y * trans.y + frustum[i].z * trans.z + frustum[i].w <= -mesh.boundingSphere.radius * maxScale)
						{
							visible = false;
							break;
						}
					}

					if (!visible) {
						cullMeshCount++;
						continue;
					}
				}

				vertexCount += mesh.GetIndicesCount();
				visibleMeshTransform.push_back(modelMatrix * transform);
			}

			if (buffer)
			{
				if (!visibleMeshTransform.size())continue;

				glBindBuffer(GL_ARRAY_BUFFER, buffer);
				glBufferData(GL_ARRAY_BUFFER, visibleMeshTransform.size() * sizeof(glm::mat4), &visibleMeshTransform[0], GL_DYNAMIC_DRAW);

				model.material.Apply(shaderCache.shader);
				mesh.material.Apply(shaderCache.shader);
				mesh.Draw(visibleMeshTransform.size());
			}
			else
			{
				// Don't use buffer
				for (int i = 0; i != visibleMeshTransform.size(); i++) {
					shaderCache.shader.SetModelMatrix(visibleMeshTransform[i]);
					model.material.Apply(shaderCache.shader);
					mesh.material.Apply(shaderCache.shader);
					mesh.Draw();
				}
			}

			glCheckError();
		}

		std::cout << "Cull Mesh:" << cullMeshCount << "\t Total Mesh:" << totalMeshCount << "\t Vetex:" << vertexCount << std::endl;
	}
}