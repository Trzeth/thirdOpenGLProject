#include "Shader.h"
#include "Renderer.h"
#include "RenderUtil.h"

Renderer::Renderer()
{
	this->uiModelTransform = glm::mat4();
	// Flip the y axis so we can use normal modelspace but position in UI space
	this->uiModelTransform[1][1] = -1.0f;
}

Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		assert(0);
		//Failed to initialize GLAD
	}
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

	bool animatable = false;//Deal with later

	RenderableHandle handle = this->entityPool.GetNewHandle(Entity(*shader.impl, modelHandle, animatable));
	return handle;
}

void Renderer::Draw()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		/*
		if (renderable.animatable) {
			for (const auto& mesh : model.meshes) {
				std::vector<glm::mat4> nodeTransforms = model.GetNodeTransforms(renderable.animName, renderable.time, renderable.context);
				if (renderable.animName.empty()) {
					// Just do bindpose
				}
				else if (mesh.impl->boneData.size() == 0) {
					// Not skinned animation
					// TODO: Actually find the node of the mesh
					modelMatrix *= nodeTransforms[1];
				}
				else {
					// Skinned animation
					std::vector<glm::mat4> boneTransforms = mesh.first.GetBoneTransforms(nodeTransforms);
					for (unsigned int j = 0; j < boneTransforms.size(); j++) {
						glUniformMatrix4fv(shaderCache.bones[j], 1, GL_FALSE, &boneTransforms[j][0][0]);
					}
				}
			}
		}

		shaderCache.shader.setMat4("model", modelMatrix);

		model.Draw(shaderCache.shader, -1);
		*/
	}
}