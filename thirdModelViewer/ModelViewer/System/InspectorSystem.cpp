#include "InspectorSystem.h"

#include <glad/glad.h>
#include  <thirdEngine/Renderer/Renderer.h>
#include <thirdEngine/Renderer/UI/UIRenderer.h>

#include "ModelViewer/Component/InspectorComponent.h"
#include "ModelViewer/Component/ModelRenderComponent.h"
#include "ModelViewer/Component/ObjectViewerComponent.h"

InspectorSystem::InspectorSystem(World& world, Renderer& renderer)
	:System(world), renderer(renderer), lastResizeTime(RESIZETHRESHOLD)
{
	require<InspectorComponent>();
}

void InspectorSystem::updateEntity(float dt, eid_t entity)
{
	InspectorComponent* inspectorComponent = world.GetComponent<InspectorComponent>(entity);

	ModelRenderComponent* modelRenderComponent = world.GetComponent<ModelRenderComponent>(inspectorComponent->viewer);
	ObjectViewerComponent* objectViewerComponent = world.GetComponent<ObjectViewerComponent>(inspectorComponent->viewer);

	const auto& curMaterial = objectViewerComponent->materialList[objectViewerComponent->currentMaterialIndex];

	if (objectViewerComponent->fileChangedFlag) {
		auto& fileInfo = objectViewerComponent->fileInfo;

		Model model = modelLoader.LoadFromFile(fileInfo.FullPath, ModelLoadingPrefab::Optimize, glm::mat4(1.0), true);
		model.GenVAO();

		std::vector<std::string> animationList = model.GetAnimationName();
		animationList.insert(animationList.begin(), std::string("bindpose"));

		fileInfo.AnimationNameList = animationList;
		objectViewerComponent->currentAnimationIndex = 0;

		Renderer::ModelHandle modelHandle = renderer.GetModelHandle(model);
		modelRenderComponent->rendererHandle = renderer.GetRenderableHandle(modelHandle,
			curMaterial.shader);
		renderer.SetRenderableMaterial(modelRenderComponent->rendererHandle, curMaterial.material);

		objectViewerComponent->fileChangedFlag = false;
	}

	if (objectViewerComponent->animationChangedFlag) {
		renderer.SetRenderableAnimation(modelRenderComponent->rendererHandle, objectViewerComponent->fileInfo.AnimationNameList[objectViewerComponent->currentAnimationIndex]);
		objectViewerComponent->animationChangedFlag = false;
	}

	if (objectViewerComponent->materialReloadFlag) {
		objectViewerComponent->materialList[objectViewerComponent->currentMaterialIndex].shader =
			shaderLoader.BuildFromFile(
				curMaterial.shaderFileInfo.vertexShaderPath,
				curMaterial.shaderFileInfo.fragmentShaderPath
			);

		objectViewerComponent->materialReloadFlag = false;
		objectViewerComponent->materialChangedFlag = true;
	}

	if (objectViewerComponent->materialChangedFlag) {
		renderer.SetRenderableShader(modelRenderComponent->rendererHandle, curMaterial.shader);
		renderer.SetRenderableMaterial(modelRenderComponent->rendererHandle, curMaterial.material);
		objectViewerComponent->materialChangedFlag = false;
	}

	// Render To Frame buffer

	if (lastResizeTime < RESIZETHRESHOLD)lastResizeTime += dt;

	auto ele = inspectorComponent->objectViewer;
	float h = ele->GetWindowHeight();
	float w = ele->GetWindowWidth();

	if (h == 0 || w == 0)return;

	if (lastResizeTime >= RESIZETHRESHOLD && (h != inspectorComponent->preWindowHeight || w != inspectorComponent->preWindowWidth))
	{
		//限制时间是因为 glTexImage2D 内存释放好像有问题 => 会延迟释放 => resize触发频繁 造成内存激增
		lastResizeTime = 0.0f;

		inspectorComponent->preWindowHeight = h;
		inspectorComponent->preWindowWidth = w;

		renderer.SetViewport(w, h);

		glBindTexture(GL_TEXTURE_2D, inspectorComponent->texColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, inspectorComponent->frameBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, inspectorComponent->renderBuffer);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);

		//更改大小前先解绑？
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, inspectorComponent->renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, inspectorComponent->frameBuffer);

	// 将它附加到当前绑定的帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, inspectorComponent->texColorBuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	renderer.Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}