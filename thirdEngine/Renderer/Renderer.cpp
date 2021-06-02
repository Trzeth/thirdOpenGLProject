#include "Shader.h"
#include "Renderer.h"
#include "RenderUtil.h"

#include "Mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Renderer/ModelLoader.h>
#include <Renderer/UI/ImGui/imgui.h>

Renderer::Renderer()
{
	this->uiModelTransform = glm::mat4(1.0f);
	// Flip the y axis so we can use normal modelspace but position in UI space
	this->uiModelTransform[1][1] = -1.0f;
}

Renderer::ShaderCache::ShaderCache(const Shader& shader)
	:shader(shader), bones(maxBones)
{
	// Bind to bindpoint 0
	glUniformBlockBinding(shader.GetID(), glGetUniformBlockIndex(shader.GetID(), "baseMatrices"), 0);

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

	//因为是单例 要不等它自动释放吧
	{
		// Uniform Buffer Object
		glGenBuffers(1, &baseMatrixUBO);

		glBindBuffer(GL_UNIFORM_BUFFER, baseMatrixUBO);
		glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4) + 5 * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, baseMatrixUBO, 0, 3 * sizeof(glm::mat4) + 5 * sizeof(glm::vec4));
		glCheckError();
	}

	{
		// Shadow
		glGenFramebuffers(1, &depthMapFBO);
		glGenFramebuffers(1, &depthStaticMapFBO);

		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenTextures(1, &depthStaticMap);
		glBindTexture(GL_TEXTURE_2D, depthStaticMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glCheckError();

		glBindFramebuffer(GL_FRAMEBUFFER, depthStaticMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthStaticMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCheckError();
	}

	debugBoundingSphere = ModelLoader().LoadFromFile("Resources/sphere.obj");
	debugBoundingSphere.data->meshes[0].GenVAO();

	depthShaderCache = ShaderCache(ShaderLoader().BuildFromFile("Shaders/depthShader.vert", "Shaders/depthShader.frag"));

	frustum.resize(6);
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

Renderer::RenderableHandle Renderer::GetRenderableHandle(const ModelHandle& modelHandle, const Shader& shader, bool isDynamic)
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

	shader.Use();
	glUniform1i(shader.GetUniformLocation("shadowMap"), 0);

	RenderableHandle handle = this->entityPool.GetNewHandle(Entity(shader, modelHandle, animatable, isDynamic));

	return handle;
}

void Renderer::ClearBuffer() const
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::ClearStaticShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, depthStaticMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	redrawDepthStaticMap = true;
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

	redrawDepthStaticMap = false;
}

void Renderer::drawStaticEntity()
{
	glCheckError();

	glBindFramebuffer(GL_FRAMEBUFFER, depthStaticMapFBO);

	// Depth Pass
	GLfloat near_plane = 1.0f, far_plane = 1000.0f;
	glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(-100.0f * dirLight.direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;

	// Calculate Frustum
	{
		glm::mat4 matrix = glm::transpose(lightSpaceMatrix);
		frustum[0] = matrix[3] + matrix[0];
		frustum[1] = matrix[3] - matrix[0];
		frustum[2] = matrix[3] + matrix[1];
		frustum[3] = matrix[3] - matrix[1];
		frustum[4] = matrix[3] + matrix[2];
		frustum[5] = matrix[3] - matrix[2];

		for (int i = 0; i != frustum.size(); i++)
			frustum[i] /= glm::length2(glm::vec3(frustum[i]));
	}

	depthShaderCache.shader.Use();
	glUniformMatrix4fv(depthShaderCache.shader.GetUniformLocation("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (auto iter = entityPool.begin(); iter != entityPool.end(); iter++) {
		Entity& renderable = iter->second;

		if (renderable.isDynamic)
		{
			continue;
		}

		std::optional<std::reference_wrapper<Model>> modelOpt = modelPool.Get(renderable.modelHandle);
		assert(modelOpt);

		Model& model = *modelOpt;
		ShaderCache& shaderCache = renderable.shaderCache;
		glm::mat4 modelMatrix = renderable.transform;

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
						glUniformMatrix4fv(depthShaderCache.bones[j], 1, GL_FALSE, &boneTransforms[j][0][0]);
					}
				}
			}

			// We assume mesh won't under animted node

			std::vector<glm::mat4> visibleMeshTransform;
			const auto& [buffer, transforms] = model.data->meshesTransform[i];
			for (const auto& transform : transforms)
			{
				totalMeshCount++;
				glCheckError();

				float t = 0.0;
				if (mesh.hasBoundingSphere)
				{
					/* Culling Per Mesh */

					bool visible = true;
					auto trans = modelMatrix * transform * glm::vec4(mesh.boundingSphere.center, 1);
					auto matrix = modelMatrix * transform;
					float maxScale = glm::max(glm::max(matrix[0][0], matrix[1][1]), matrix[2][2]);
					t = maxScale;

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
				glCheckError();

				vertexCount += mesh.GetIndicesCount();
				visibleMeshTransform.push_back(modelMatrix * transform);
			}

			if (buffer)
			{
				if (!visibleMeshTransform.size())continue;

				glBindBuffer(GL_ARRAY_BUFFER, buffer);
				glBufferData(GL_ARRAY_BUFFER, visibleMeshTransform.size() * sizeof(glm::mat4), &visibleMeshTransform[0], GL_DYNAMIC_DRAW);

				model.material.Apply(depthShaderCache.shader);
				mesh.material.Apply(depthShaderCache.shader);

				mesh.Draw(visibleMeshTransform.size());
			}
			else
			{
				// Don't use buffer
				for (int i = 0; i != visibleMeshTransform.size(); i++) {
					depthShaderCache.shader.SetModelMatrix(visibleMeshTransform[i]);
					model.material.Apply(depthShaderCache.shader);
					mesh.material.Apply(depthShaderCache.shader);

					mesh.Draw();
				}
			}

			glCheckError();
		}
	}
	glCheckError();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::drawInternal(RenderSpace space)
{
	ImGui::Begin("Dirlight");

	ImGui::DragFloat3("Direction", glm::value_ptr(dirLight.direction));
	ImGui::DragFloat3("Ambient", glm::value_ptr(dirLight.ambient));
	ImGui::DragFloat3("Diffuse", glm::value_ptr(dirLight.diffuse));
	ImGui::DragFloat3("Specular", glm::value_ptr(dirLight.specular));

	ImGui::End();

	if (redrawDepthStaticMap)
	{
		// Static Shadow Pass
		drawStaticEntity();
	}

	// 0 Dynamic Shadow Depth Pass 1 Normal Draw
	for (const int drawIndex : {0, 1}) {
		switch (drawIndex)
		{
		case 0:
		{
			// Dynamic Depth Pass
			GLfloat near_plane = 1.0f, far_plane = 1000.0f;
			glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
			glm::mat4 lightView = glm::lookAt(-100.0f * dirLight.direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			lightSpaceMatrix = lightProjection * lightView;

			// Calculate Frustum
			{
				glm::mat4 matrix = glm::transpose(lightSpaceMatrix);
				frustum[0] = matrix[3] + matrix[0];
				frustum[1] = matrix[3] - matrix[0];
				frustum[2] = matrix[3] + matrix[1];
				frustum[3] = matrix[3] - matrix[1];
				frustum[4] = matrix[3] + matrix[2];
				frustum[5] = matrix[3] - matrix[2];

				for (int i = 0; i != frustum.size(); i++)
					frustum[i] /= glm::length2(glm::vec3(frustum[i]));
			}

			depthShaderCache.shader.Use();
			glUniformMatrix4fv(depthShaderCache.shader.GetUniformLocation("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

			glBindFramebuffer(GL_READ_FRAMEBUFFER, depthStaticMapFBO);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, depthMapFBO);
			glBlitFramebuffer(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT, 0, 0, SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glCheckError();

			break;
		}
		case 1:
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, viewportWidth, viewportHeight);
			// Calculate Frustum
			{
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

			glBindBuffer(GL_UNIFORM_BUFFER, baseMatrixUBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 1, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix));

			// 注意对齐
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3 + sizeof(glm::vec4) * 0, sizeof(glm::vec3), glm::value_ptr(viewPos));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3 + sizeof(glm::vec4) * 1, sizeof(glm::vec3), glm::value_ptr(dirLight.direction));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3 + sizeof(glm::vec4) * 2, sizeof(glm::vec3), glm::value_ptr(dirLight.ambient));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3 + sizeof(glm::vec4) * 3, sizeof(glm::vec3), glm::value_ptr(dirLight.diffuse));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3 + sizeof(glm::vec4) * 4, sizeof(glm::vec3), glm::value_ptr(dirLight.specular));
		}
		break;
		}

		for (auto iter = entityPool.begin(); iter != entityPool.end(); iter++) {
			Entity& renderable = iter->second;

			if (renderable.space != space ||
				(!renderable.isDynamic && drawIndex == 0)) {
				continue;
			}

			std::optional<std::reference_wrapper<Model>> modelOpt = modelPool.Get(renderable.modelHandle);
			assert(modelOpt);

			Model& model = *modelOpt;
			ShaderCache& shaderCache = renderable.shaderCache;
			glm::mat4 modelMatrix = renderable.transform;

			if (drawIndex == 1)
			{
				shaderCache.shader.Use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, depthMap);
			}

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
							if (drawIndex == 0)
								glUniformMatrix4fv(depthShaderCache.bones[j], 1, GL_FALSE, &boneTransforms[j][0][0]);
							else if (drawIndex == 1)
								glUniformMatrix4fv(shaderCache.bones[j], 1, GL_FALSE, &boneTransforms[j][0][0]);
						}
					}
				}

				// We assume mesh won't under animted node

				std::vector<glm::mat4> visibleMeshTransform;
				const auto& [buffer, transforms] = model.data->meshesTransform[i];
				for (const auto& transform : transforms)
				{
					totalMeshCount++;
					glCheckError();

					float t = 0.0;
					if (mesh.hasBoundingSphere)
					{
						/* Culling Per Mesh */

						bool visible = true;
						auto trans = modelMatrix * transform * glm::vec4(mesh.boundingSphere.center, 1);
						auto matrix = modelMatrix * transform;
						float maxScale = glm::max(glm::max(matrix[0][0], matrix[1][1]), matrix[2][2]);
						t = maxScale;

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
					glCheckError();

					/*
					glm::mat4 trans = glm::scale(glm::mat4(1.0), glm::vec3(t));
					trans[3] = transform * modelMatrix * glm::vec4(mesh.boundingSphere.center, 1);
					shaderCache.shader.SetModelMatrix(trans);
					debugBoundingSphere.data->meshes[0].material.Apply(shaderCache.shader);
					debugBoundingSphere.data->meshes[0].Draw();

					shaderCache.shader.SetModelMatrix(trans * glm::mat4(1.5f));
					debugBoundingSphere.data->meshes[0].material.Apply(shaderCache.shader);
					debugBoundingSphere.data->meshes[0].Draw();
					*/

					vertexCount += mesh.GetIndicesCount();
					visibleMeshTransform.push_back(modelMatrix * transform);
				}

				if (buffer)
				{
					if (!visibleMeshTransform.size())continue;

					glBindBuffer(GL_ARRAY_BUFFER, buffer);
					glBufferData(GL_ARRAY_BUFFER, visibleMeshTransform.size() * sizeof(glm::mat4), &visibleMeshTransform[0], GL_DYNAMIC_DRAW);

					if (drawIndex == 0)
					{
						model.material.Apply(depthShaderCache.shader);
						mesh.material.Apply(depthShaderCache.shader);
					}
					else if (drawIndex == 1)
					{
						model.material.Apply(shaderCache.shader);
						mesh.material.Apply(shaderCache.shader);
					}
					mesh.Draw(visibleMeshTransform.size());
				}
				else
				{
					// Don't use buffer
					for (int i = 0; i != visibleMeshTransform.size(); i++) {
						if (drawIndex == 0)
						{
							depthShaderCache.shader.SetModelMatrix(visibleMeshTransform[i]);
							model.material.Apply(depthShaderCache.shader);
							mesh.material.Apply(depthShaderCache.shader);
						}
						else if (drawIndex == 1) {
							shaderCache.shader.SetModelMatrix(visibleMeshTransform[i]);
							model.material.Apply(shaderCache.shader);
							mesh.material.Apply(shaderCache.shader);
						}

						mesh.Draw();
					}
				}

				glCheckError();
			}

			std::cout << "Cull Mesh:" << cullMeshCount << "\t Total Mesh:" << totalMeshCount << "\t Vetex:" << vertexCount << std::endl;
		}
	}
}