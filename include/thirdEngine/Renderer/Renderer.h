#pragma once
#include <unordered_map>
#include <map>

#include "Model.h"
#include "HandlePool.h"

/*! A global directional light. */
struct DirLight
{
	glm::vec3 direction;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

/*! Different render spaces. */
enum class RenderSpace
{
	/*! Render in worldspace. */
	World,
};

constexpr unsigned int maxPointLights = 64;
constexpr unsigned int maxBones = 100;

class Renderer
{
public:
	/*! Shader cache. Stores a shader along with its uniform locations. */
	struct ShaderCache
	{
		ShaderCache() { };
		ShaderCache(const Shader& shader);

		Shader shader;
		std::vector<GLuint> bones;
	};

	/*! A renderable, stored internally in the renderer.
	Renderables can be equated to entities. They reference a model and a shader, but have
	their own transforms. */
	struct Entity
	{
		Entity(const Shader& shader, HandlePool<Model>::Handle modelHandle, bool animatable, bool isDynamic)
			: shaderCache(shader), modelHandle(modelHandle), space(RenderSpace::World),
			animatable(animatable), loopAnimation(false), autoUpdate(true), time(0),
			transform(1.0f), culling(true), isDynamic(isDynamic)
		{ }

		HandlePool<Model>::Handle modelHandle;
		bool isDynamic;

		bool culling;

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

		bool autoUpdate;
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

	void SetRenderableAnimation(const RenderableHandle& handle, const std::string& animName, float time = 0.0f, bool autoPlay = true, bool loop = true);

	void SetRenderableAnimationTime(const RenderableHandle& handle, float time);

	void SetRenderableModelHandle(const RenderableHandle& handle, const ModelHandle& modelHandle);

	void SetRenderableRenderSpace(const RenderableHandle& handle, RenderSpace space);

	void SetRenderableTransform(const RenderableHandle& handle, const glm::mat4& transform);

	void SetRenderableMaterial(const RenderableHandle& handle, const Material& material);

	void SetRenderableShader(const RenderableHandle& handle, const Shader& shader);

	void SetViewport(int w, int h);

	void Clear();

	// low left corner
	void SetViewport(int x, int y, int w, int h);

	void GenVAO();

	int GetViewportWidth() const;
	int GetViewportHeight() const;

	float GetRenderableAnimationTime(const RenderableHandle& handle);

	ModelHandle GetModelHandle(Model model);

	RenderableHandle GetRenderableHandle(const ModelHandle& modelHandle, const Shader& shader, bool isDynamic = false);

	void ClearBuffer() const;

	void ClearStaticShadowMap();

	void Draw();

	void Update(float dt);
private:
	void drawStaticEntity();
	void drawInternal(RenderSpace space);

	DirLight dirLight;

	std::unordered_map<uint64_t, ShaderCache> shaderMap;

	HandlePool<Model> modelPool;
	HandlePool<Entity> entityPool;

	glm::mat4 uiModelTransform;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 viewPos;
	glm::mat4 lightSpaceMatrix;

	Model debugBoundingSphere;

	std::vector<glm::vec4> frustum;

	GLuint baseMatrixUBO;

	// Full combined shadow map
	GLuint depthMapFBO;
	GLuint depthMap;

	// Draw on first render
	bool redrawDepthStaticMap;
	GLuint depthStaticMapFBO;
	GLuint depthStaticMap;

	ShaderCache depthShaderCache;

	const GLuint SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

	int viewportWidth;
	int viewportHeight;
};
