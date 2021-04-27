#pragma once
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/ComponentConstructor.h>

#include <thirdEngine/Renderer/Renderer.h>

struct ModelRenderComponent :public Component
{
	ModelRenderComponent() :rendererHandle(nullptr) { }
	Renderer::RenderableHandle rendererHandle;
};

class ModelRenderComponentConstructor :public ComponentConstructor {
public:
	ModelRenderComponentConstructor(Renderer& renderer, const Renderer::ModelHandle& modelHandle, Shader&& shader, const std::string& defaultAnimation = "")
		: renderer(renderer), modelHandle(modelHandle), shader(std::move(shader)), defaultAnimation(defaultAnimation) { }

	~ModelRenderComponentConstructor() {
		printf("De\n");
	}

	virtual ComponentConstructorInfo Construct(World& world, eid_t parent, void* userinfo)const;

	virtual void Finish(World& world, eid_t entity);
private:
	Renderer& renderer;
	Shader shader;
	Renderer::ModelHandle modelHandle;
	std::string defaultAnimation;
};