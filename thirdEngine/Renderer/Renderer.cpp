#include "Shader.h"
#include "Renderer.h"
#include "RenderUtil.h"

#include "Mesh.h"
#include "MeshImpl.h"

Renderer::Renderer()
{
	this->uiModelTransform = glm::mat4();
	// Flip the y axis so we can use normal modelspace but position in UI space
	this->uiModelTransform[1][1] = -1.0f;
}

Renderer::ShaderCache::ShaderCache(const ShaderImpl& shader)
	:shader(shader), pointLights(maxPointLights), bones(maxBones)
{
	for (unsigned int i = 0; i < pointLights.size(); i++) {
		PointLightCache& light = this->pointLights[i];
		std::stringstream sstream;
		sstream << "pointLight[" << i << "]";
		light.constant = shader.GetUniformLocation((sstream.str() + ".constant").c_str());
		light.linear = shader.GetUniformLocation((sstream.str() + ".linear").c_str());
		light.quadratic = shader.GetUniformLocation((sstream.str() + ".quadratic"));
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
}

void Renderer::SetDirLight(const DirLight& dirLight)
{
	this->dirLight = dirLight;
}

void Renderer::SetProjectionMatrix(const glm::mat4& transform)
{
	projectionMatrix = transform;
}

void Renderer::SetViewMatrix(const glm::mat4& transform)
{
	this->viewMatrix = transform;
}

void Renderer::SetRenderableAnimation(const RenderableHandle& handle, const std::string& animName, bool loop)
{
	std::optional<std::reference_wrapper<Entity>> renderableOpt = entityPool.Get(handle);
	if (!renderableOpt) {
		return;
	}

	Entity& renderable = *renderableOpt;
	renderable.animName = animName;
	renderable.time = 0.0f;
	renderable.loopAnimation = loop;
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

void Renderer::SetViewport(int w, int h)
{
	SetViewport(0, 0, w, h);
}

void Renderer::SetViewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);

	viewportWidth = w;
	viewportHeight = h;
}

void Renderer::SetFBO(GLuint FBO)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

GLuint Renderer::GetFBO() const
{
	return FBO;
}

int Renderer::GetViewportWidth() const
{
	return viewportWidth;
}

int Renderer::GetViewportHeight() const
{
	return viewportHeight;
}

Renderer::ModelHandle Renderer::GetModelHandle(const Model& model)
{
	return modelPool.GetNewHandle(model);
}

Renderer::RenderableHandle Renderer::GetRenderableHandle(const ModelHandle& modelHandle, const Shader& shader)
{
	auto shaderIter = shaderMap.find(shader.impl->GetID());
	if (shaderIter == shaderMap.end()) {
		auto iterPair = shaderMap.emplace(std::make_pair(shader.impl->GetID(), ShaderCache(*shader.impl)));
	}

	std::optional<std::reference_wrapper<Model>> modelOpt = modelPool.Get(modelHandle);
	if (!modelOpt) {
		return entityPool.InvalidHandle;
	}

	Model& model = *modelOpt;

	bool animatable = (model.animationData.animations.size() > 0);

	RenderableHandle handle = this->entityPool.GetNewHandle(Entity(*shader.impl, modelHandle, animatable));

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

	drawInternal(RenderSpace_World);
}

void Renderer::Update(float dt)
{
	for (auto iter = entityPool.begin(); iter != entityPool.end(); iter++) {
		Entity& renderable = iter->second;
		std::string animName = renderable.animName;
		if (animName.size() == 0) {
			continue;
		}

		renderable.time += dt;

		std::optional<std::reference_wrapper<Model>> modelOpt = modelPool.Get(iter->second.modelHandle);
		assert(modelOpt);

		// Deal with Later

		Model& model = *modelOpt;
		auto& animationMap = model.animationData.animations;
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
	for (auto iter = shaderMap.begin(); iter != shaderMap.end(); iter++) {
		const ShaderCache& shaderCache = iter->second;

		shaderCache.shader.Use();

		//??? Viewpos lightSpaceMartix skyBox Shader如何处理

		shaderCache.shader.SetProjectionMatrix(projectionMatrix);
		shaderCache.shader.SetViewMatrix(viewMatrix);

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

		//只考虑了骨骼动画！！
		for (int i = 0; i != model.meshes.size(); i++)
		{
			//Do Anim once per mesh
			if (renderable.animatable) {
				std::vector<glm::mat4> nodeTransforms = model.GetNodeTransforms(renderable.animName, renderable.time, renderable.context);

				if (model.meshes[i].impl->boneData.size() == 0) {
					// Not skinned animation
				}
				else
				{
					//Skinned animation
					std::vector<glm::mat4> boneTransforms = model.meshes[i].GetBoneTransforms(nodeTransforms);
					for (unsigned int j = 0; j < boneTransforms.size(); j++) {
						glUniformMatrix4fv(shaderCache.bones[j], 1, GL_FALSE, &boneTransforms[j][0][0]);
					}
				}

				// In case of mesh under animated node
				for (const int node : model.animationData.meshNodeId[i]) {
					shaderCache.shader.SetModelMatrix(modelMatrix * nodeTransforms[node]);

					model.meshes[i].material.Apply(shaderCache.shader);
					model.meshes[i].Draw();
					glCheckError();
				}
			}
			else
			{
				for (const glm::mat4& transform : model.meshesTransform[i]) {
					shaderCache.shader.SetModelMatrix(modelMatrix * transform);
					model.meshes[i].material.Apply(shaderCache.shader);
					model.meshes[i].Draw();
					glCheckError();
				}
			}
		}
	}
}