#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <map>

#include "Model.h"
#include "HandlePool.h"

/*! A global directional light. */
struct DirLight
{
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

/*! Different render spaces. */
enum RenderSpace
{
	/*! Render in worldspace. */
	RenderSpace_World,
};

constexpr unsigned int maxPointLights = 64;
constexpr unsigned int maxBones = 100;

class Renderer
{
public:
	/*! Shader cache. Stores a shader along with its uniform locations. */
	struct ShaderCache
	{
		ShaderCache(const ShaderImpl& shader);

		struct PointLightCache
		{
			GLuint position;

			GLuint constant;
			GLuint linear;
			GLuint quadratic;

			GLuint ambient;
			GLuint diffuse;
			GLuint specular;
		};

		struct DirLightCache
		{
			GLuint direction;
			GLuint ambient;
			GLuint diffuse;
			GLuint specular;
		};

		ShaderImpl shader;
		DirLightCache dirLight;
		std::vector<PointLightCache> pointLights;
		std::vector<GLuint> bones;
		GLuint pointLightCount;
	};

	/*! A renderable, stored internally in the renderer.
	Renderables can be equated to entities. They reference a model and a shader, but have
	their own transforms. */
	struct Entity
	{
		Entity(const ShaderImpl& shader, HandlePool<Model>::Handle modelHandle, bool animatable)
			: shaderCache(shader), modelHandle(modelHandle), animatable(animatable), space(RenderSpace_World), loopAnimation(false), time(0), transform(1.0f) { }

		HandlePool<Model>::Handle modelHandle;
		ShaderCache shaderCache;
		glm::mat4 transform;

		AnimationContext context;

		/*! Which space to render the renderable in. Defaults to RenderSpace_World. */
		RenderSpace space;

		/*! Whether or not this renderable can be animated. */
		bool animatable;

		/*! Current animation playing. */
		std::string animName;

		/*! Current time within the animation. */
		float time;

		/*! Whether or not to loop the current animation. */
		bool loopAnimation;
	};

	typedef HandlePool<Model>::Handle ModelHandle;
	typedef HandlePool<Entity>::Handle RenderableHandle;
	//typedef HandlePool<PointLight>::Handle PointLightHandle;

	Renderer();

	void Initialize(int width, int height);

	void SetDirLight(const DirLight& dirLight);

	void SetProjectionMatrix(const glm::mat4& transform);

	void SetViewMatrix(const glm::mat4& transform);

	void SetViewPos(const glm::vec3& pos);

	void SetRenderableAnimation(const RenderableHandle& handle, const std::string& animName, bool loop = true);

	void SetRenderableAnimationTime(const RenderableHandle& handle, float time);

	void SetRenderableModelHandle(const RenderableHandle& handle, const ModelHandle& modelHandle);

	void SetRenderableRenderSpace(const RenderableHandle& handle, RenderSpace space);

	void SetRenderableTransform(const RenderableHandle& handle, const glm::mat4& transform);

	void SetRenderableShader(const RenderableHandle& handle, const Shader& shader);

	void SetRenderableMaterial(const RenderableHandle& handle, const std::shared_ptr<Material>& material);

	void SetViewport(int w, int h);

	// low left corner
	void SetViewport(int x, int y, int w, int h);

	int GetViewportWidth() const;
	int GetViewportHeight() const;

	ModelHandle GetModelHandle(const Model& model);

	RenderableHandle GetRenderableHandle(const ModelHandle& modelHandle, const Shader& shader);

	void ClearBuffer()const;

	void Draw();

	void Update(float dt);
private:
	void drawInternal(RenderSpace space);

	DirLight dirLight;

	std::unordered_map<uint64_t, ShaderCache> shaderMap;

	HandlePool<Model> modelPool;
	HandlePool<Entity> entityPool;

	glm::mat4 uiModelTransform;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 viewPos;

	int viewportWidth;
	int viewportHeight;
};
