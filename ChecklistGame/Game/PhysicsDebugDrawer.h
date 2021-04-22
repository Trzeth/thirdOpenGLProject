#pragma once
#include <memory>
#include <vector>

#include <box2d/box2d.h>
#include <thirdEngine/Renderer/Camera.h>

class PhysicsDebugDrawer :public b2Draw
{
public:
	PhysicsDebugDrawer();
	~PhysicsDebugDrawer();

	void Initialize();
	void Reset();
	void Draw();

	void SetCamera(Camera* camera);

	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

	virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

	virtual void DrawTransform(const b2Transform& xf) override;

	virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;
private:
	void insert(float x, float z, float cx, float cy, float cz);

	float PredefineY = 5;

	Camera* camera;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colors;
	unsigned i;
	unsigned currentBufferSize;

	struct Impl;
	std::unique_ptr<Impl> impl;
};
