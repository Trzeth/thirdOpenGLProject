#include "PhysicsDebugDrawer.h"

#include <glad/glad.h>

#include <thirdEngine/Renderer/Shader.h>

struct PhysicsDebugDrawer::Impl
{
	//因为是单例 要不等它自动释放吧
	GLuint VAO, VBO, VBO_color;
	Shader shader;
};

PhysicsDebugDrawer::PhysicsDebugDrawer()
	: currentBufferSize(0),
	i(0),
	impl(new Impl),
	camera(nullptr)
{ }

PhysicsDebugDrawer::~PhysicsDebugDrawer()
{
}

void PhysicsDebugDrawer::Initialize()
{
	ShaderLoader shaderLoader;
	impl->shader = shaderLoader.BuildFromFile("Shaders/colors.vert", "Shaders/colors.frag");

	glGenVertexArrays(1, &impl->VAO);
	glGenBuffers(1, &impl->VBO);
	glGenBuffers(1, &impl->VBO_color);

	glBindVertexArray(impl->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, impl->VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, impl->VBO_color);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PhysicsDebugDrawer::Reset()
{
	i = 0;
	currentBufferSize = 0;
	vertices.clear();
	colors.clear();
}

void PhysicsDebugDrawer::Draw()
{
	if (camera == nullptr) {
		return;
	}

	impl->shader.Use();
	impl->shader.SetModelMatrix(glm::mat4(1.0f));
	impl->shader.SetProjectionMatrix(camera->GetProjectionMatrix());
	impl->shader.SetViewMatrix(camera->GetViewMatrix());

	glBindVertexArray(impl->VAO);

	if (vertices.size() > currentBufferSize) {
		currentBufferSize = vertices.size();

		glBindBuffer(GL_ARRAY_BUFFER, impl->VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, impl->VBO_color);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * 3 * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glLineWidth(5);
	glDrawArrays(GL_LINES, 0, vertices.size());
	glLineWidth(1);

	glBindVertexArray(0);
}

void PhysicsDebugDrawer::SetCamera(Camera* camera)
{
	this->camera = camera;
}

void PhysicsDebugDrawer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	for (int i = 0; i != vertexCount; i++) {
		DrawSegment(vertices[(i - 1 + vertexCount) % vertexCount], vertices[i], color);
	}
}

void PhysicsDebugDrawer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	DrawPolygon(vertices, vertexCount, color);
}

void PhysicsDebugDrawer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	b2Vec2 up(center), down(center), left(center), right(center);
	up.x += radius;
	down.x -= radius;
	left.y -= radius;
	right.y += radius;

	DrawSegment(up, left, color);
	DrawSegment(left, down, color);
	DrawSegment(down, right, color);
	DrawSegment(right, up, color);
}

void PhysicsDebugDrawer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
	DrawCircle(center, radius, color);
}

void PhysicsDebugDrawer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	insert(p1.x, p1.y, color.r, color.g, color.b);
	insert(p2.x, p2.y, color.r, color.g, color.b);
}

void PhysicsDebugDrawer::DrawTransform(const b2Transform& xf)
{
}

void PhysicsDebugDrawer::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
}

void PhysicsDebugDrawer::insert(float x, float z, float cx, float cy, float cz)
{
	if (i >= vertices.size()) {
		vertices.emplace_back(x, PredefineY, z);
		colors.emplace_back(cx, cy, cz);
	}
	else {
		vertices[i].x = x;
		vertices[i].y = PredefineY;
		vertices[i].z = z;
		colors[i].x = cx;
		colors[i].y = cy;
		colors[i].z = cz;
	}
	i++;
}