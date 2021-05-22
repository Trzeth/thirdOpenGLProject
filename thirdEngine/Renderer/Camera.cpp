#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/orthonormalize.hpp>
#include <glm/gtx/projection.hpp>

Camera::Camera()
	: fieldOfView(glm::radians(45.0f)),
	width(0),
	height(0),
	nearClip(0.1f),
	farClip(10000),
	inverseViewMatrix(1.0f)
{ }

Camera::Camera(float fieldOfView, unsigned int width, unsigned int height, float nearClip, float farClip)
	: fieldOfView(fieldOfView),
	width(width),
	height(height),
	nearClip(nearClip),
	farClip(farClip),
	inverseViewMatrix(1.0f)
{ }

glm::mat4 Camera::GetViewMatrix()
{
	//return inverseViewMatrix;
	return glm::inverse(inverseViewMatrix);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	if (height == 0)
		return glm::mat4(1.0f);

	return glm::perspective(fieldOfView, (float)width / (float)height, nearClip, farClip);
}

glm::mat4 Camera::GetViewMatrixOrtho()
{
	return glm::mat4(1.0f);
}

glm::mat4 Camera::GetProjectionMatrixOrtho()
{
	// Reverse y direction since that's the way 2D applications normally have it
	return glm::ortho(0.0f, (float)width, (float)height, 0.0f);
}

glm::vec2 Camera::WorldToScreenPoint(glm::vec3 worldPoint)
{
	glm::vec4 clipSpacePos = this->GetProjectionMatrix() * (this->GetViewMatrix() * glm::vec4(worldPoint, 1.0f));
	glm::vec3 ndcSpacePos = glm::vec3(clipSpacePos) / clipSpacePos.w;
	ndcSpacePos.y = -ndcSpacePos.y;
	glm::vec2 windowSpacePos = (glm::vec2(ndcSpacePos) + glm::vec2(1.0f)) / 2.0f * glm::vec2(width, height);
	return windowSpacePos;
}

void Camera::SetInverseViewMatrix(const glm::mat4& inverseViewMatrix)
{
	this->inverseViewMatrix = inverseViewMatrix;
}

glm::mat4 Camera::GetInverseViewMatrix()
{
	return this->inverseViewMatrix;
}

void Camera::SetFOV(float fov)
{
	fieldOfView = fov;
}

void Camera::SetScreenSize(unsigned int w, unsigned int h)
{
	width = w;
	height = h;
}

void Camera::SetClip(float near, float far)
{
	nearClip = near;
	farClip = far;
}